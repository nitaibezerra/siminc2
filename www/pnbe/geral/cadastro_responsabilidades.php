<?
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if( !$pflcod && !$usucpf )
{
	?><font color="red">Requisi��o inv�lida</font><?
	exit();
}

$sqlResponsabilidadesPerfil = "SELECT tr.*
                                FROM 
                                    pnbe.tprperfil p
                                    INNER JOIN pnbe.tiporesponsabilidade tr ON (p.tprcod = tr.tprcod)
                                WHERE 
                                    tprsnvisivelperfil = TRUE AND 
                                    p.pflcod = '%s'
                                ORDER BY 
                                    tr.tprdsc";

$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
$responsabilidadesPerfil = $db->carregar($query);


if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil)<1) {
	print "<font color='red'>N�o foram encontrados registros</font>";
}
else {
	foreach ($responsabilidadesPerfil as $rp) {
		//
		// monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (a��o, programas, etc)
		$sqlRespUsuario = "";
		switch ($rp["tprsigla"]) {	
			case "E": // Autor
				$aca_prg = "'Editoras'";
				$sqlRespUsuario = "SELECT 
                                                    e.ediid as codigo,
                                                    e.edirazaosocial as descricao
                                                  FROM 
                                                    pnbe.editora e INNER JOIN pnbe.usuarioresponsabilidade ur
                                                    ON (e.ediid = ur.ediid)
                                                  WHERE
                                                    ur.rpustatus = 'A'
                                                    AND ur.usucpf = '%s' 
                                                    AND ur.pflcod = '%s'";
			break;
		}
		
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		
		$respUsuario = $db->carregar($query);
		if (!$respUsuario || @count($respUsuario)<1) {
			print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>N�o existem $aca_prg para este Perfil.</font>";
		}
		else {
		?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
	<tr>
	  <td colspan="3"><?=$rp["tprdsc"]?></td>
	</tr>
	<tr style="color:#000000;">
      <td valign="top" width="12">&nbsp;</td>
	  <td valign="top">C�digo</td>
	  <td valign="top">Descri��o</td>
    </tr>
		<?
			foreach ($respUsuario as $ru) {
		?>
	<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
      <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	  <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?=$ru["codigo"];?>
	  </td>
	  <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?=$ru["descricao"]?></td>
	</tr>
		<?
		}
		?>
	<tr>
	  <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">
	    Total: (<?=@count($respUsuario)?>)
	  </td>
	</tr>
</table>
	<?
		}
	}
}
$db->close();
exit();
?>