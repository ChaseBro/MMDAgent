/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2007                            */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Authors:  Alan W Black (awb@cs.cmu.edu)                   */
/*                Date:  November 2007                                   */
/*************************************************************************/
/*                                                                       */
/*  Implementation of Clustergen, Statistical Parameter Synthesizer in   */
/*  Flite                                                                */
/*                                                                       */
/*  A statistical corpus based synthesizer.                              */
/*  See Black, A. (2006), CLUSTERGEN: A Statistical Parametric           */
/*  Synthesizer using Trajectory Modeling", Interspeech 2006 - ICSLP,    */
/*  Pittsburgh, PA.                                                      */
/*  http://www.cs.cmu.edu/~awb/papers/is2006/IS061394.PDF                */
/*                                                                       */
/*  Uses MLSA for resynthesis and MLPG for smoothing                     */
/*  mlsa and mlpg come from Festvox's VC code (which came in turn        */
/*  came from NITECH's HTS                                               */
/*                                                                       */
/*************************************************************************/

#include "cst_cg.h"
#include "cst_spamf0.h"
#include "cst_hrg.h"
#include "cst_utt_utils.h"
#include "cst_audio.h"

CST_VAL_REGISTER_TYPE(cg_db,cst_cg_db)

static cst_utterance *cg_make_hmmstates(cst_utterance *utt);
static cst_utterance *cg_make_params(cst_utterance *utt);
static cst_utterance *cg_predict_params(cst_utterance *utt);
static cst_utterance *cg_resynth(cst_utterance *utt);

void delete_cg_db(cst_cg_db *db)
{
    int i,j;

    if (db->freeable == 0)
        return;  /* its in the data segment, so not freeable */

    /* Woo Hoo!  We're gonna free this garbage with a big mallet */
    /* In spite of what the const qualifiers say ... */
    cst_free((void *)db->name);

    for (i=0; db->types && db->types[i]; i++)
        cst_free((void *)db->types[i]);
    cst_free((void *)db->types);

    for (i=0; db->f0_trees && db->f0_trees[i]; i++)
        delete_cart((cst_cart *)(void *)db->f0_trees[i]);
    cst_free((void *)db->f0_trees);

    for (i=0; db->param_trees0 && db->param_trees0[i]; i++)
        delete_cart((cst_cart *)(void *)db->param_trees0[i]);
    cst_free((void *)db->param_trees0);

    for (i=0; db->param_trees1 && db->param_trees1[i]; i++)
        delete_cart((cst_cart *)(void *)db->param_trees1[i]);
    cst_free((void *)db->param_trees1);

    for (i=0; db->param_trees2 && db->param_trees2[i]; i++)
        delete_cart((cst_cart *)(void *)db->param_trees2[i]);
    cst_free((void *)db->param_trees2);

    if (db->spamf0)
    {
        delete_cart((cst_cart *)(void *)db->spamf0_accent_tree);
        delete_cart((cst_cart *)(void *)db->spamf0_phrase_tree);
        for (i=0; i< db->num_frames_spamf0_accent; i++)
            cst_free((void *)db->spamf0_accent_vectors[i]);
        cst_free((void *)db->spamf0_accent_vectors);
    }

    for (i=0; i<db->num_frames0; i++)
        cst_free((void *)db->model_vectors0[i]);
    cst_free((void *)db->model_vectors0);
    for (i=0; i<db->num_frames1; i++)
        cst_free((void *)db->model_vectors1[i]);
    cst_free((void *)db->model_vectors1);
    for (i=0; i<db->num_frames2; i++)
        cst_free((void *)db->model_vectors2[i]);
    cst_free((void *)db->model_vectors2);

    cst_free((void *)db->model_min);
    cst_free((void *)db->model_range);

    for (i=0; db->dur_stats && db->dur_stats[i]; i++)
    {
        cst_free((void *)db->dur_stats[i]->phone);
        cst_free((void *)db->dur_stats[i]);
    }
    cst_free((void *)db->dur_stats);
    delete_cart((void *)db->dur_cart);

    for (i=0; db->phone_states && db->phone_states[i]; i++)
    {
        for (j=0; db->phone_states[i][j]; j++)
            cst_free((void *)db->phone_states[i][j]);
        cst_free((void *)db->phone_states[i]);
    }
    cst_free((void *)db->phone_states);

    cst_free((void *)db->dynwin);

    for (i=0; i<db->ME_num; i++)
        cst_free((void *)db->me_h[i]);
    cst_free((void *)db->me_h);

    cst_free((void *)db);
}

/* */
cst_utterance *cg_synth(cst_utterance *utt)
{
    cst_cg_db *cg_db;
    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));

    cg_make_hmmstates(utt);
    cg_make_params(utt);
    cg_predict_params(utt);
    if (cg_db->spamf0)
    {
	cst_spamf0(utt);
    }
    cg_resynth(utt);

    return utt;
}

static float cg_state_duration(cst_item *s, cst_cg_db *cg_db)
{
    float zdur, dur;
    const char *n;
    int i, x;

    zdur = val_float(cart_interpret(s,cg_db->dur_cart));
    n = item_feat_string(s,"name");

    for (x=i=0; cg_db->dur_stats[i]; i++)
    {
        if (cst_streq(cg_db->dur_stats[i]->phone,n))
        {
            x=i;
            break;
        }
    }
    if (!cg_db->dur_stats[i])  /* unknown type name */
        x = 0;

    dur = (zdur*cg_db->dur_stats[x]->stddev)+cg_db->dur_stats[x]->mean;

    /*    dur = 1.2 * (float)exp((float)dur); */

    return dur;
}

static cst_utterance *cg_make_hmmstates(cst_utterance *utt)
{
    /* Build HMM state structure below the segment structure */
    cst_cg_db *cg_db;
    cst_relation *hmmstate, *segstate;
    cst_item *seg, *s, *ss;
    const char *segname;
    int sp,p;

    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));
    hmmstate = utt_relation_create(utt,"HMMstate");
    segstate = utt_relation_create(utt,"segstate");

    for (seg = utt_rel_head(utt,"Segment"); seg; seg=item_next(seg))
    {
        ss = relation_append(segstate,seg);
        segname = item_feat_string(seg,"name");
        for (p=0; cg_db->phone_states[p]; p++)
            if (cst_streq(segname,cg_db->phone_states[p][0]))
                break;
        if (cg_db->phone_states[p] == NULL)
            p = 0;  /* unknown phoneme */
        for (sp=1; cg_db->phone_states[p][sp]; sp++)
        {
            s = relation_append(hmmstate,NULL);
            item_add_daughter(ss,s);
            item_set_string(s,"name",cg_db->phone_states[p][sp]);
            item_set_int(s,"statepos",sp);
        }
    }

    return utt;
}

static cst_utterance *cg_make_params(cst_utterance *utt)
{
    /* puts in the frame items */
    /* historically called "mcep" but can actually be any random vectors */
    cst_cg_db *cg_db;
    cst_relation *mcep, *mcep_link;
    cst_item *s, *mcep_parent, *mcep_frame;
    int num_frames;
    float start, end;
    float dur_stretch, tok_stretch;

    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));
    mcep = utt_relation_create(utt,"mcep");
    mcep_link = utt_relation_create(utt,"mcep_link");
    end = 0.0;
    num_frames = 0;
    dur_stretch = get_param_float(utt->features,"duration_stretch", 1.0);

    for (s = utt_rel_head(utt,"HMMstate"); s; s=item_next(s))
    {
        start = end;
        tok_stretch = ffeature_float(s,"R:segstate.parent.R:SylStructure.parent.parent.R:Token.parent.local_duration_stretch");
        if (tok_stretch == 0)
            tok_stretch = 1.0;
        end = start + (tok_stretch*dur_stretch*cg_state_duration(s,cg_db));
        item_set_float(s,"end",end);
        mcep_parent = relation_append(mcep_link, s);
        for ( ; (num_frames * cg_db->frame_advance) <= end; num_frames++ )
        {
            mcep_frame = relation_append(mcep,NULL);
            item_add_daughter(mcep_parent,mcep_frame);
            item_set_int(mcep_frame,"frame_number",num_frames);
            item_set(mcep_frame,"name",item_feat(mcep_parent,"name"));
        }
    }

    /* Copy duration up onto Segment relation */
    for (s = utt_rel_head(utt,"Segment"); s; s=item_next(s))
        item_set(s,"end",ffeature(s,"R:segstate.daughtern.end"));

    utt_set_feat_int(utt,"param_track_num_frames",num_frames);

    return utt;
}

#if CG_OLD
static int voiced_frame(cst_item *m)
{
    const char *ph_vc;
    const char *ph_cvox;

    ph_vc = ffeature_string(m,"R:mcep_link.parent.R:segstate.parent.ph_vc");
    ph_cvox = ffeature_string(m,"R:mcep_link.parent.R:segstate.parent.ph_cvox");

    if (cst_streq("-",ph_vc) &&
        cst_streq("-",ph_cvox))
        return 0; /* unvoiced */
    else
        return 1; /* voiced */
}
#endif

static int voiced_frame(cst_item *m)
{
    const char *ph_vc;
    const char *ph_name;

    ph_vc = ffeature_string(m,"R:mcep_link.parent.R:segstate.parent.ph_vc");
    ph_name = ffeature_string(m,"R:mcep_link.parent.R:segstate.parent.name");

    if (cst_streq(ph_name,"pau"))
        return 0; /* unvoiced */
    else if (cst_streq("+",ph_vc))
        return 1; /* voiced */
    else if (item_feat_float(m,"voicing") > 0.5)
        /* Even though the range is 0-10, I *do* mean 0.5 */
        return 1; /* voiced */
    else
        return 0; /* unvoiced */
}

static void cg_smooth_F0(cst_utterance *utt,cst_cg_db *cg_db,
                         cst_track *param_track)
{
    /* Smooth F0 and mark unnoived frames as 0.0 */
    cst_item *mcep;
    int i, c;
    float l, s;
    float mean, stddev;

    l = 0.0;
    for (i=0; i<param_track->num_frames-1; i++)
    {
        c = 0; s = 0;
        if (l > 0.0)
        {
            c++; s+=l;
        }
        if (param_track->frames[i+1][0] > 0.0)
        {
            c++; s+=param_track->frames[i+1][0];
        }
        l = param_track->frames[i][0];
        if (param_track->frames[i][0] > 0.0)
        {
            c++; s+=param_track->frames[i][0];
            param_track->frames[i][0] = s/c;
        }
    }

    mean = get_param_float(utt->features,"int_f0_target_mean", cg_db->f0_mean);
    mean *= get_param_float(utt->features,"f0_shift", 1.0);
    stddev = 
        get_param_float(utt->features,"int_f0_target_stddev", cg_db->f0_stddev);
    
    for (i=0,mcep=utt_rel_head(utt,"mcep"); mcep; i++,mcep=item_next(mcep))
    {
        if (voiced_frame(mcep))
        {
            /* scale the F0 -- which normally wont change it at all */
            param_track->frames[i][0] = 
                (((param_track->frames[i][0]-cg_db->f0_mean)/cg_db->f0_stddev) 
                 *stddev)+mean;
            /* Some safety checks */
            if (param_track->frames[i][0] < 50)
                param_track->frames[i][0] = 50;
            if (param_track->frames[i][0] > 700)
                param_track->frames[i][0] = 700;
        }
        else /* Unvoice it */
            param_track->frames[i][0] = 0.0;
    }

    return;
}

static cst_utterance *cg_predict_params(cst_utterance *utt)
{
    cst_cg_db *cg_db;
    cst_track *param_track;
    cst_track *str_track = NULL;
    cst_item *mcep;
    const cst_cart *mcep_tree, *f0_tree;
    int i,j,f,p,fd,o;
    const char *mname;
    float f0_val;
    int fff;
    int extra_feats = 0;

    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));
    param_track = new_track();
    if (cg_db->do_mlpg) /* which should be the default */
        fff = 1;  /* copy details with stddevs */
    else
        fff = 2;  /* copy details without stddevs */

    extra_feats = 1;  /* voicing */
    if (cg_db->mixed_excitation)
    {
        extra_feats += 5;
        str_track = new_track();
        cst_track_resize(str_track,
                         utt_feat_int(utt,"param_track_num_frames"),
                         5);
    }
    
    cst_track_resize(param_track,
                     utt_feat_int(utt,"param_track_num_frames"),
                     (cg_db->num_channels0/fff)-
                       (2 * extra_feats));/* no voicing or str */
    for (i=0,mcep=utt_rel_head(utt,"mcep"); mcep; i++,mcep=item_next(mcep))
    {
        mname = item_feat_string(mcep,"name");
        for (p=0; cg_db->types[p]; p++)
            if (cst_streq(mname,cg_db->types[p]))
                break;
        if (cg_db->types[0] == NULL)
            p=0; /* if there isn't a matching tree, use the first one */

        /* Predict F0 */
        f0_tree = cg_db->f0_trees[p];
        f0_val = val_float(cart_interpret(mcep,f0_tree));
        param_track->frames[i][0] = f0_val;
        /* what about stddev ? */

        if (cg_db->multimodel)
        {   /* MULTI model */
            f = val_int(cart_interpret(mcep,cg_db->param_trees0[p]));
            fd = val_int(cart_interpret(mcep,cg_db->param_trees1[p]));
            item_set_int(mcep,"clustergen_param_frame",f);

            param_track->frames[i][0] = 
                (param_track->frames[i][0]+
                 CG_MODEL_VECTOR(cg_db,model_vectors0,f,0)+
                 CG_MODEL_VECTOR(cg_db,model_vectors1,fd,0))/3.0;
            for (j=2; j<param_track->num_channels; j++)
                param_track->frames[i][j] = 
                    (CG_MODEL_VECTOR(cg_db,model_vectors0,f,(j)*fff)+
                     CG_MODEL_VECTOR(cg_db,model_vectors1,fd,(j)*fff))/2.0;
            if (cg_db->mixed_excitation)
            {
                o = j;
                for (j=0; j<5; j++)
                {
                    str_track->frames[i][j] =
                        (CG_MODEL_VECTOR(cg_db,model_vectors0,f,(o+(2*j))*fff)+
                         CG_MODEL_VECTOR(cg_db,model_vectors1,fd,(o+(2*j))*fff))/2.0;
                }
            }
        }
        else  
        {   /* SINGLE model */
            /* Predict Spectral */
            mcep_tree = cg_db->param_trees0[p];
            f = val_int(cart_interpret(mcep,mcep_tree));
            item_set_int(mcep,"clustergen_param_frame",f);

            param_track->frames[i][0] = 
                (param_track->frames[i][0]+
                 CG_MODEL_VECTOR(cg_db,model_vectors0,f,0))/2.0;

            for (j=2; j<param_track->num_channels; j++)
                param_track->frames[i][j] =
                    CG_MODEL_VECTOR(cg_db,model_vectors0,f,(j)*fff);

            if (cg_db->mixed_excitation)
            {
                o = j;
                for (j=0; j<5; j++)
                {
                    str_track->frames[i][j] =
                        CG_MODEL_VECTOR(cg_db,model_vectors0,f,(o+(2*j))*fff);
                }
            }
        }

        /* last coefficient is average voicing for cluster */
        item_set_float(mcep,"voicing",
                       CG_MODEL_VECTOR(cg_db,model_vectors0,f,
                                       cg_db->num_channels0-2));

        param_track->times[i] = i * cg_db->frame_advance;
    }

    cg_smooth_F0(utt,cg_db,param_track);

    utt_set_feat(utt,"param_track",track_val(param_track));
    if (cg_db->mixed_excitation)
        utt_set_feat(utt,"str_track",track_val(str_track));

    return utt;
}

static cst_utterance *cg_resynth(cst_utterance *utt)
{
    cst_cg_db *cg_db;
    cst_wave *w;
    cst_track *param_track;
    cst_track *str_track = NULL;
    cst_track *smoothed_track;
    const cst_val *streaming_info_val;
    cst_audio_streaming_info *asi = NULL;

    streaming_info_val=get_param_val(utt->features,"streaming_info",NULL);
    if (streaming_info_val)
    {
        asi = val_audio_streaming_info(streaming_info_val);
        asi->utt = utt;
    }

    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));
    param_track = val_track(utt_feat_val(utt,"param_track"));
    if (cg_db->mixed_excitation)
        str_track = val_track(utt_feat_val(utt,"str_track"));

    if (cg_db->do_mlpg)
    {
        smoothed_track = mlpg(param_track, cg_db);
        w = mlsa_resynthesis(smoothed_track,str_track,cg_db,asi);
        delete_track(smoothed_track);
    }
    else
        w=mlsa_resynthesis(param_track,str_track,cg_db,asi);

    if (w == NULL)
    {
        /* Synthesis Failed, probably because it was interrupted */
        utt_set_feat_int(utt,"Interrupted",1);
        w = new_wave();
    }

    utt_set_wave(utt,w);

    return utt;
}



