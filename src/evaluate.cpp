#include "evaluate.h"

/*****************************************************************************
 * HarLog: http://arimaa.com/arimaa/forum/cgi/YaBB.cgi?board=devTalk;action=display;num=1062013358
 *
 * Here is the material evaluation function used by my bot. It is the
 * difference between the result of the following computation for one side and
 * that of the other: 
 * 
 * For every friendly non-rabbit with no stronger enemy piece add 2/Q. 
 *
 * For every other friendly non-rabbit add 1/(Q+number_of_stronger_enemy_pieces). 
 *
 * Finally, add G*ln(number_of_friendly_rabbits*number_of_total_friendly_pieces). 
 * 
 * The chosen values for the parameters are: 
 *
 * Q=1.447530126 
 * G=0.6314442034 
 * 
 * This makes the material evaluation function completely indifferent towards a
 * trade of a dog for two rabbits.
 ****************************************************************************/


// float harlog(const GameState& gs, const Color for_color)
// {
    // int num_stronger_pieces[nPieces];
    // gs.get_num_stronger_pieces(num_stronger_pieces, for_color);
    
    // float score = 0.;
    // for(unsigned int i=C; i < nPieces; ++i) {
        // if (!num_stronger_pieces[i]) {
            // score += 2. / Q;
        // } else {
            // score += 1. / (Q + num_stronger_pieces[i]);
        // }
    // }
    // score += G * approx_log(gs.get_num_pieces(R, for_color) * gs.get_total_num_pieces(for_color));
// }

/*****************************************************************************
 * DAPE (Optimized Coefficients) 
 * 
 * With coefficients optimized by my system, DAPE outperformed the other
 * algorithms and not by a small amount!  How much of this is due to DAPE being
 * a better description of Arimaa's mechanics (pieces evaluated with respect to
 * how many other pieces they can beat) and how much is due to the fact that
 * DAPE has seven tweakable coefficients is not clear.  While the end
 * performance was fairly consistent, the actual coefficients varied quite a
 * bit; each time I ran the optimizer it found a slightly different solution
 * for DAPE.   With seven interdependent coefficients, there are bound to be
 * many local minima in the error function. 
 *
 * Here are the coefficients (using Toby's terminology from this post and the
 * code on Janzert's calculator) at the end of the best-performing optimizing
 * run: 
 * 
 * A=27.9307 
 * S=0.7833 
 * E=0.4282 
 * AR = -0.46 
 * BR = -1.0683 
 * AN = 428.7 
 * BN = 0.9755 
 * (Error = 1870.34) 
 * 
 * The higher values of S and E (relative to the defaults) are telling us that,
 * like with FAME, having a larger number of pieces is more important than
 * having stronger pieces (relative to what we think, anyhow.)   AN and AR are
 * not described in 99of9's original post on DAPE but are alluded to later in
 * the thread and can be found in the code on Janzert's FAME/DAPE web page. 
 ****************************************************************************/


/* The JS code from Janzert's website.
 *
function harlog(pcs, rbts) {
        var Q = 1.447530126;
        var G = 0.6314442034;
        var normal = 0.12507009891301202;

        var gp = pcs[0].slice(0)
        var sp = pcs[1].slice(0)

        var gr = rbts[0]
        var sr = rbts[1]

        var gs = 0;
        var pix = 0;
        var eix = 0;
        while (pix < gp.length)
        {
            while (eix < sp.length && gp[pix] < sp[eix])
            {
                eix++;
            }

            if (eix == 0)
            {
                gs += 2/Q;
            } else {
                gs += 1 / (Q + eix);
            }
            pix++;
        }
        gs += G * Math.log(gr * (gr + gp.length));

        pix = 0;
        eix = 0;
        var ss = 0;
        while (pix < sp.length)
        {
            while (eix < gp.length && sp[pix] < gp[eix])
            {
                eix++;
            }

            if (eix == 0)
            {
                ss += 2/Q;
            } else {
                ss += 1 / (Q + eix);
            }
            pix++;
        }
        ss += G * Math.log(sr * (sr + sp.length));

        return (gs - ss) / normal;
    }

*/

/*
function dape(pcs, rbts, CA, CS, CE, AR, BR, AN, BN, normal) {
        var gp = pcs[0].slice(0)
        var sp = pcs[1].slice(0)

        var gr = rbts[0]
        var sr = rbts[1]

        gs = 0
        ss = 0
	    var equal
	    var above

	  for (mig=0; mig<16; mig++) {
	    above = 0
	    equal = 0
	  
	    if (mig < gp.length)
	      g = gp[mig]
	    else if (mig < (gp.length+gr))
	      g = 1
	    else
	      g = 0

	    if (g!=0) {
	      for (mis=0; mis<16; mis++) {
	        if (mis < sp.length)
	          s = sp[mis]
	        else if (mis < (sp.length+sr))
	          s = 1
	        else
	          s = 0

	        if (s>g) above++
	        if (s==g) equal++
	      }

	      gs += CA/(CS+CE*equal+above)
	    }
	  }

	  for (mis=0; mis<16; mis++) {
	    above = 0
	    equal = 0

	    if (mis < sp.length)
	      s = sp[mis]
	    else if (mis < (sp.length+sr))
	      s = 1
	    else
	      s = 0

	    if (s!=0) {
	      for (mig=0; mig<16; mig++) {
	        if (mig < gp.length)
	          g = gp[mig]
	        else if (mig < (gp.length+gr))
	          g = 1
	        else
	          g = 0

	        if (g>s) above++
	        if (g==s) equal++
	      }

	      ss += CA/(CS+CE*equal+above)
	    }
	  }

	  gs -= (AR*Math.pow(BR,gr))
	  ss -= (AR*Math.pow(BR,sr))

	  gs -= (AN*Math.pow(BN,gp.length+gr))
	  ss -= (AN*Math.pow(BN,sp.length+sr))

	  return (gs-ss)/normal
    }
*/
