<?php 
header("Content-Type: text/html; charset=ISO-8859-1",true); 

#$_POST[ord] ? $cosord = (int) $_POST[ord] : $cosord = 0;

$sql = sprintf("SELECT
					il.*,
					udl.unddid,
					udl.undddsc
				FROM
					cte.laboratorio l JOIN cte.itenslaboratorio il USING(labid)
					JOIN cte.unidademedidadetalhamento udl USING(unddid) 
				WHERE
				
					l.undid = %d", $_POST[undid]);
$dados = $db->carregar($sql);

//dump($dados);
if (is_array($dados)):
	$t = "<table 
				border=\"0\"
				align=\"left\"
				cellpadding=\"0\"
				cellspacing=\"1\"
				width=\"100%\"
			>";
	for ($i=0; $i < count($dados); $i++):
		$cosord++;
		$totvlr += (int) ($dados[$i][ilbqtditens] * $dados[$i][ilbvlrunitario]);
		$t .= '<TR style="backgroundColor:\'#f0f0f0\';">
	           	<TD width="24" align="center" >&nbsp</TD>
	           	<TD width="46" align="center" ><input type="text" maxlength="3" class="normal" name="composicao[cosord][]" style="width: 25px; text-align: center;" value="'. $cosord .'" readonly="readonly" /> <input type="hidden"  name="composicao[ilbid][]"  value="'. $dados[$i][ilbid] .'" /> </TD>
	           	<TD ><input type="text" maxlength="70" class="normal" name="composicao[cosdsc][]" style="width: 420px;" value="' . str_replace('"', "&#".ord('"').';', $dados[$i][ilbdesc]) . '" readonly="readonly"/></TD>
	           	<TD width="146" ><select name="composicao[unddid][]" class="normal" style="width: 140px;"><option value="'.$dados[$i][unddid].'">' . str_replace('"', "&#".ord('"').';',$dados[$i][undddsc]) . '</option></select></TD>
	           	<TD width="46" align="right" ><input type="text" maxlength="9" class="normal" name="composicao[cosqtd][]" style="width: 40px; text-align: right;" value="'.$dados[$i][ilbqtditens].'" readonly="readonly" /><input type="hidden" name="cosqtd[]" value="'.$dados[$i][ilbqtditens].'" readonly="readonly" /></TD>
	           	<TD width="66" align="right" ><input type="text" maxlength="12" class="normal" name="composicao[cosvlruni][]" style="width: 60px; text-align: right;" value="'. number_format($dados[$i][ilbvlrunitario],2,',','.') . '" readonly="readonly"/></TD> 		
	           	<TD width="106" align="right" id="valTot_'.$i.'" >'.number_format($dados[$i][ilbqtditens] * $dados[$i][ilbvlrunitario],2,',','.').'</TD>
			   </TR>';
	endfor;
	$t .= "<TR style=\"display:none;\">
				<TD><input type=\"hidden\" name=\"vlrtotal\" value=\"".$totvlr."\" ></TD>	
		   </TR>
		</table>";
	
	die ($t);	
endif;
die('not_itens')
?>
