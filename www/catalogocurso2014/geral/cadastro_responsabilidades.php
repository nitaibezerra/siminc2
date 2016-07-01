<?php
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadastro_usuario_elaboracao_responsabilidades.php
   
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if(!$pflcod && !$usucpf) { ?>
	<font color="red">Requisição inválida</font>
<?php
	exit();
}

$sqlResponsabilidadesPerfil = "SELECT 		DISTINCT tr.*
							   FROM 		catalogocurso2014.tprperfil p
							   INNER JOIN 	catalogocurso2014.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
							   WHERE 		tprsnvisivelperfil = TRUE AND p.pflcod = '%s' AND p.prfano = {$_SESSION['exercicio']} AND tr.tprano = {$_SESSION['exercicio']}
							   ORDER BY 	tr.tprdsc";
$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
$responsabilidadesPerfil = $db->carregar($query);
if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil)<1) {
	print "<font color='red'>Não foram encontrados registros</font>";
}
else {
	foreach ($responsabilidadesPerfil as $rp) {
		//
		// monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (ação, programas, etc)
		$sqlRespUsuario = "";
		switch ($rp["tprsigla"]) {
			case "U": // Coordenações 
				$aca_prg = "Coordenações";
				$sqlRespUsuario = "SELECT 		c.coordid AS codigo, 
												c.coordsigla||' - '||c.coorddesc AS descricao, 
												c.coordstatus AS status
								   FROM 		catalogocurso2014.coordenacao c 
								   INNER JOIN 	catalogocurso2014.usuarioresponsabilidade u ON u.coordid = c.coordid
								   WHERE	    u.usucpf = '%s' AND u.pflcod = '%s' AND u.rpustatus='A' AND c.coorano = {$_SESSION['exercicio']}";
				break;
			case "C": // Cursos
				$aca_prg = "Cursos";
				$sqlRespUsuario = "SELECT 		c.curid AS codigo, 
												curdesc AS descricao,
												u.rpustatus AS status
								   FROM 		catalogocurso2014.curso c
								   INNER JOIN 	catalogocurso2014.usuarioresponsabilidade u ON u.curid = c.curid
								   WHERE		curstatus = 'A' AND u.usucpf = '%s' AND u.pflcod = '%s' AND u.rpustatus='A' AND c.curano = {$_SESSION['exercicio']}";
				break;
			default:
				break;
		}
		
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		$respUsuario = $db->carregar($query);
		if (!$respUsuario || @count($respUsuario)<1) {
			//print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem associações a este Perfil.</font>";
		} else { ?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
	<tr>
		<td colspan="3"><?php echo $rp["tprdsc"]; ?></td>
	</tr>
	<tr style="color:#000000;">
    	<td valign="top" width="12">&nbsp;</td>
	  	<td valign="top">Código</td>
	  	<td valign="top">Descrição</td>
    </tr>
	<?php foreach ($respUsuario as $ru) { ?>
	<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
    	<td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	  	<td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?php if($rp["tprsigla"]=='A'){?><a href="simec_er.php?modulo=principal/acao/cadacao&acao=C&acaid=<?php echo $ru["acaid"]; ?>&prgid=<?php echo $ru["prgid"]; ?>"><?php echo $ru["codigo"]; ?></a><?php } else { print $ru["codigo"]; }?></td>
	  	<td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?php echo $ru["descricao"]; ?></td>
	</tr>
	<?php }	?>
	<tr>
		<td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">Total: (<?php echo @count($respUsuario); ?>)</td>
	</tr>
</table>
	<?php
		}
	}
	
	$teste = $db->carregar("SELECT DISTINCT * FROM catalogocurso2014.usuarioresponsabilidade WHERE usucpf = '{$usucpf}' AND pflcod = {$pflcod} AND rpustatus = 'A'");
	if (!$teste) {
		print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem associações a este Perfil.</font>";
	}
}
$db->close();
exit();
?>