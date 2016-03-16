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

$sqlResponsabilidadesPerfil = "SELECT 
									tr.*
								FROM 
									contratos.tprperfil p
									INNER JOIN 
										contratos.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
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
			case "A": // A��O
				$aca_prg = "A��es Associadas";
				$sqlRespUsuario = "SELECT a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod AS codigo, a.acadsc AS descricao, a.prgid, a.acaid, a.acacod, u.rpustatus AS status
					FROM usuarioresponsabilidade u 
					INNER JOIN acao a ON a.acaid = u.acaid
					WHERE a.prgano = '".$_SESSION['exercicio']."' and u.usucpf = '%s' AND u.pflcod = '%s' AND u.rpustatus='A' ORDER BY a.prgcod, a.acacod, a.unicod, a.loccod";
			break;
			case "P": // PROGRAMAS 
				$aca_prg = "Programas Associados";
				$sqlRespUsuario = "SELECT p.prgcod AS codigo, p.prgdsc AS descricao, u.rpustatus AS status
					FROM usuarioresponsabilidade u 
					INNER JOIN programa p ON p.prgid = u.prgid
					WHERE p.prgano = '".$_SESSION['exercicio']."' and u.usucpf = '%s' AND u.pflcod = '%s' AND u.rpustatus='A'";
			break;
			case "C": // projetos especiais
				$aca_prg = "Unidades Associados";
				$sqlRespUsuario = "SELECT 
										ung.hspid AS codigo,
										CASE
											WHEN mun.mundescricao IS NOT NULL THEN  
												hspabrev || ' - ' || hspdsc || ' - ' || mun.mundescricao || '/' || mun.estuf 
											ELSE
												hspabrev || ' - ' || hspdsc
										END AS descricao, 
										u.rpustatus AS status
									FROM contratos.usuarioresponsabilidade u
									INNER JOIN contratos.hospital ung ON ung.hspid = u.hspid 
									LEFT JOIN territoriosgeo.municipio mun ON mun.muncod = ung.muncod 
									WHERE 
										u.usucpf = '%s' AND 
										u.pflcod = '%s' AND 
										u.rpustatus='A'";
				break;
			case "E": // projetos especiais 
				$aca_prg = "Projetos Associados";
				$sqlRespUsuario = "SELECT p.pjecod AS codigo, p.pjedsc AS descricao, u.rpustatus AS status
					FROM contratos.usuarioresponsabilidade u 
					INNER JOIN contratos.projetoespecial p ON p.pjeid = u.pjeid
					WHERE p.prsano = '".$_SESSION['exercicio']."' and u.usucpf = '%s' AND u.pflcod = '%s' AND u.rpustatus='A'";
			break;
			case "I": // unidades
				$aca_prg = "UASG Associadas";
				$sqlRespUsuario = "
					SELECT ua.usgcod as codigo, usgdsc as descricao FROM contratos.usuarioresponsabilidade ur 
						INNER JOIN contratos.uasg ua on ur.usgid = ua.usgid
						INNER JOIN seguranca.perfil pfl on pfl.pflcod = ur.pflcod 
					WHERE
						ur.rpustatus = 'A' and
						ur.usucpf = '%s' and
						pfl.pflcod = '%s'
				";
			break;			
			case "G": // unidades gestoras
				$aca_prg = "Unidades Gestoras associadas";
				$sqlRespUsuario = "
									SELECT 
										uni.ungdsc as descricao, 
										uni.ungcod as codigo
									FROM 
										contratos.usuarioresponsabilidade ur 
										INNER JOIN public.unidadegestora uni on
											uni.ungcod = ur.ungcod and
											uni.ungstatus = 'A'
										inner join seguranca.perfil pfl on
											pfl.pflcod = ur.pflcod
									where
										ur.prsano = '".$_SESSION['exercicio']."' and
										ur.rpustatus = 'A' and
										ur.usucpf = '%s' and
										pfl.pflcod = '%s'
				";
			break;				
		}
		
		if(!$sqlRespUsuario) continue;
		
		$query 		 = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		$respUsuario = $db->carregar($query);
		
		if (!$respUsuario || @count($respUsuario)<1) {
			print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>N�o existem $aca_prg a este Perfil.</font>";
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
	  <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?if ($rp["tprsigla"]=='A'){?><a href="contratos.php?modulo=principal/acao/cadacao&acao=C&acaid=<?=$ru["acaid"]?>&prgid=<?=$ru["prgid"]?>"><?=$ru["codigo"]?></a><?} else {print $ru["codigo"];}?></td>
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