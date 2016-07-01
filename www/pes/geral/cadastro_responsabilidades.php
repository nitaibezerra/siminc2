<?

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if(!$pflcod && !$usucpf) {
	?><font color="red">Requisição inválida</font><?
	exit();
}

$sqlResponsabilidadesPerfil = "SELECT
								tr.*
							   FROM
							   	elabrev.tprperfil p
							    INNER JOIN elabrev.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
							   WHERE
							    tprsnvisivelperfil = TRUE AND
							    p.pflcod = '%s'
							   ORDER BY
							    tr.tprdsc";
$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
$responsabilidadesPerfil = (array) $db->carregar($query);

if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil)<1) {
	print "<font color='red'>Não foram encontrados registros</font>";
}else {
	foreach ($responsabilidadesPerfil as $rp) {
		// monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (ação, programas, etc)
		$sqlRespUsuario = "";
		switch ($rp["tprsigla"]) {
			case "A": // AÇÃO
				$aca_prg = "Ações Associadas";
				$sqlRespUsuario = "SELECT DISTINCT
								    a.prgcod || '.' || a.acacod  AS codigo, a.acadsc AS descricao, a.prgid, a.acaid, a.acacod, u.rpustatus AS status
								   FROM
								    elabrev.usuarioresponsabilidade u
								    INNER JOIN elabrev.ppaacao_proposta a ON a.acaid = u.acaid
								   WHERE
								    u.usucpf = '%s' AND
								    u.pflcod = '%s' AND
								    ur.prsano = '%s' AND
								    u.rpustatus='A'
								   ORDER BY
								    a.prgcod,
								    a.acacod";
			break;
			case "P": // PROGRAMAS
				$aca_prg = "Programas Associados";
				$sqlRespUsuario = "SELECT DISTINCT
									p.prgcod AS codigo, p.prgdsc AS descricao, u.rpustatus AS status
								   FROM
								    elabrev.usuarioresponsabilidade u
								    INNER JOIN elabrev.ppaprograma_proposta p ON p.prgid = u.prgid
								   WHERE
								    u.usucpf = '%s' AND
								    u.pflcod = '%s' AND
								    ur.prsano = '%s' AND
								    u.rpustatus='A'";
			break;
			case "U": // Unidades
				$aca_prg = "Unidades Associadas";
				$sqlRespUsuario = "SELECT DISTINCT
								   u.unicod AS codigo, u.unidsc AS descricao, ur.rpustatus AS status
								   FROM
								    elabrev.usuarioresponsabilidade ur
									INNER JOIN unidade u ON u.unicod = ur.unicod
								   WHERE
								    ur.usucpf = '%s' AND
								    ur.pflcod = '%s' AND
								    ur.prsano = '%s' AND
								    ur.rpustatus='A'";
			break;
			case "G": // Unidades
				$aca_prg = "Unidades Associadas";
				$sqlRespUsuario = "	SELECT DISTINCT
								   		u.ungcod AS codigo, u.ungdsc AS descricao, ur.rpustatus AS status
								   	FROM
								    	elabrev.usuarioresponsabilidade ur
									INNER JOIN public.unidadegestora u ON u.ungcod = ur.ungcod
								   	WHERE
									    ur.usucpf = '%s' AND
									    ur.pflcod = '%s' AND
									    ur.prsano = '%s' AND
									    ur.rpustatus='A'";
			break;
			case "O": // Unidades
				$aca_prg = "Coordenações Vinculadas";
				$sqlRespUsuario = "SELECT DISTINCT
										u.cooid AS codigo, u.coodsc AS descricao, ur.rpustatus AS status
									FROM
										elabrev.usuarioresponsabilidade ur
									INNER JOIN elabrev.coordenacao u ON u.cooid = ur.cooid
									WHERE
										ur.usucpf = '%s' AND
										ur.pflcod = '%s' AND
										ur.prsano = '%s' AND
										ur.rpustatus='A'";
			break;
			case "S": // Unidades
				$aca_prg = "Secretarias Vinculadas";
				$sqlRespUsuario = "SELECT DISTINCT
										u.ungcod AS codigo, u.ungdsc AS descricao, ur.rpustatus AS status
									FROM
										elabrev.usuarioresponsabilidade ur
									INNER JOIN public.unidadegestora u ON u.ungcod = ur.ungcod
									WHERE
										ur.usucpf = '%s' AND
										ur.pflcod = '%s' AND
										ur.prsano = '%s' AND
										ur.rpustatus='A'";
			break;
			case "D": // Unidades
				$aca_prg = "Diretorias Vinculadas";
				$sqlRespUsuario = "SELECT DISTINCT
										u.dircod AS codigo, u.dirdsc AS descricao, ur.rpustatus AS status
									FROM
										elabrev.usuarioresponsabilidade ur
									INNER JOIN elabrev.diretoria u ON u.dircod = ur.dircod
									WHERE
										ur.usucpf = '%s' AND
										ur.pflcod = '%s' AND
										ur.prsano = '%s' AND
										ur.rpustatus='A'";
			break;
			case "C": // Unidades
				$aca_prg = "Secretarias Vinculadas";
				$sqlRespUsuario = "SELECT DISTINCT
										u.ungcod AS codigo, u.ungdsc AS descricao, ur.rpustatus AS status
									FROM
										elabrev.usuarioresponsabilidade ur
									INNER JOIN public.unidadegestora u ON u.ungcod = ur.ungcod
									WHERE
										ur.usucpf = '%s' AND
										ur.pflcod = '%s' AND
										ur.prsano = '%s' AND
										ur.rpustatus='A'";
			break;
		}

		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod, $_SESSION['exercicio']));
		$respUsuario = (array) $db->carregar($query);

		if (!$respUsuario[0] || @count($respUsuario) < 1) {
			print "<center><font color='red'>Não existem $aca_prg a este Perfil.</font></center>";
		}else {
		?>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
				<tr>
				  <td colspan="3"><?=$rp["tprdsc"]?></td>
				</tr>
				<tr style="color:#000000;">
			      <td valign="top" width="12">&nbsp;</td>
				  <td valign="top">Código</td>
				  <td valign="top">Descrição</td>
			    </tr>
			    <?php if($respUsuario): ?>
					<?
					foreach ($respUsuario as $ru) {
					?>
					<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
				      <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
					  <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?if ($rp["tprsigla"]=='A'){?><a href="simec_er.php?modulo=principal/acao/cadacao&acao=C&acaid=<?=$ru["acaid"]?>&prgid=<?=$ru["prgid"]?>"><?=$ru["codigo"]?></a><?} else {print $ru["codigo"];}?></td>
					  <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?=$ru["descricao"]?></td>
					</tr>
					<?
					}
					?>
				<?php else: ?>
					<tr>
						<td colspan="3">Nunhum vínculo encontrado.</td>
					</tr>
				<?php endif; ?>
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