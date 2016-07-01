<?php
function montarAbasArray2($itensMenu, $url = false)
{
    $url = $url ? $url : $_SERVER['REQUEST_URI'];
    
    if (is_array($itensMenu)) {
        $rs = $itensMenu;
    } else {
        global $db;
        $rs = $db->carregar($itensMenu);
    }

    $menu    = '<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint">'
             . '<tr>'
             . '<td>'
             . '<table cellpadding="0" cellspacing="0" align="left">'
             . '<tr>';

    $nlinhas = count($rs) - 1;

    for ($j = 0; $j <= $nlinhas; $j++) {
        extract($rs[$j]);
        if ($url != $link && $j == 0)
            $gifaba = 'aba_nosel_ini.gif';
        elseif ($url == $link && $j == 0)
            $gifaba = 'aba_esq_sel_ini.gif';
        elseif ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
            $gifaba = 'aba_dir_sel.gif';
        elseif ($url != $link)
            $gifaba = 'aba_nosel.gif';
        elseif ($url == $link)
            $gifaba = 'aba_esq_sel.gif';

        if ($url == $link) {
            $giffundo_aba = 'aba_fundo_sel.gif';
            $cor_fonteaba = '#000055';
        } else {
            $giffundo_aba = 'aba_fundo_nosel.gif';
            $cor_fonteaba = '#4488cc';
        }
        $menu .= '<td height="20" valign="top"><img src="../../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td>'
               . '<td height="20" align="center" valign="middle" background="../../imagens/'.$giffundo_aba.'" style="color:'.$cor_fonteaba.'; padding-left: 10px; padding-right: 10px;">';

               
		$aba = substr( $url, 43, strpos( substr( $url, 43 ), "&" ) );
		$aba = $aba ? $aba : "apresentacao";
		$rsCaminho =  substr( $rs[$j][1], 21, strpos( substr( $rs[$j][1], 21 ), "&" ) );
		
		$estiloTextoAba = $aba == $rsCaminho ? "color: #7e8e47; font-weight: bold" : "color: #4488cc;";

		if ($link != $url) {
        	$descricao = $rs[$j][0];
        	$link = $rs[$j][1];
            $menu .= '<a  href="'. $link .'" style="'.$estiloTextoAba.'" title="">'.$descricao.'</a>';
        } else {
            $menu .= $descricao . '</td>';
        }
    }
    
    if ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
        $gifaba = 'aba_dir_sel_fim.gif';
    else
        $gifaba = 'aba_nosel_fim.gif';

    $menu .= '<td height="20" valign="top"><img src="../../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td></tr></table></td></tr></table>';

    return $menu;
}
