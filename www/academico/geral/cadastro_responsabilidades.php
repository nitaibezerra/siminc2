<?
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

if(!$pflcod && !$usucpf) {
	?><font color="red">Requisição inválida</font><?
	eixt();
}

$sqlResponsabilidadesPerfil = "SELECT tr.*
							   FROM academico.tprperfil p
							   INNER JOIN academico.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
							   WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
							   ORDER BY tr.tprdsc";
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
			case "O": // Tipo de Ensino 
				$aca_prg = "Tipo de Ensino";
				$sqlRespUsuario = "SELECT DISTINCT 
									o.orgid AS codigo, 
									o.orgdesc AS descricao, 
									ur.rpustatus AS status
								   FROM
								    academico.usuarioresponsabilidade ur 
								   INNER JOIN 
								    academico.orgao o ON o.orgid = ur.orgid
								   WHERE
								    ur.usucpf = '%s' AND 
								    ur.pflcod = '%s' AND 
								    ur.rpustatus='A'";
				break;
			case "U":
				$aca_prg = "Unidades Associadas";
				$sqlRespUsuario = "SELECT 
									DISTINCT 
									e.entid AS codigo, 
									e.entnome ||' - '||(SELECT
														 orgdesc 
														FROM
														 academico.orgao 
														WHERE orgid = CASE
																		WHEN funid = 12 THEN 1
																		WHEN funid = 11 OR funid = 14 THEN 2
																		WHEN funid = 102 THEN 3
																	  END) AS descricao, 
									ur.rpustatus AS status
								   FROM
								    academico.usuarioresponsabilidade ur 
								   INNER JOIN 
								    entidade.entidade e ON e.entid = ur.entid
								   INNER JOIN
								   	entidade.funcaoentidade ef ON ef.entid = e.entid
								   								  AND ef.funid IN (11,12,14,102)
								   WHERE
								    ur.usucpf = '%s' AND 
								    ur.pflcod = '%s' AND 
								    ur.rpustatus='A'";
				
				break;	
			case "C":
                            $sqlRespUsuario = "
                                SELECT  DISTINCT e.entid as codigo,
                                        initcap(e.entnome) as descricao
                                FROM entidade.entidade e

                                INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid AND fe.funid IN (17,18)
                                INNER JOIN academico.usuarioresponsabilidade ur ON ur.entid = e.entid AND ur.rpustatus = 'A'
                                
                                WHERE ur.rpustatus='A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'

                                ORDER BY descricao
                            ";
				break;		
			case "R":
				$sqlRespUsuario = "SELECT 
									  c.cooid as codigo,
									  c.coodsc as descricao,
									  ur.rpustatus AS status
									FROM 
									  academico.coordenacao c
						              inner join academico.usuarioresponsabilidade ur on ur.cooid = c.cooid and c.coostatus = 'A'
									WHERE
								    ur.usucpf = '%s' AND 
								    ur.pflcod = '%s' AND 
								    ur.rpustatus='A'";
				break;
			case "D":
				$sqlRespUsuario = "SELECT 
									  	d.dirid as codigo,
									  	d.dirdsc as descricao
									FROM 
									  	academico.diretoria d
						              	inner join academico.usuarioresponsabilidade ur on ur.dirid = d.dirid and d.dirstatus = 'A'
									WHERE
								    	ur.usucpf = '%s' AND 
								    	ur.pflcod = '%s' AND 
								    	ur.rpustatus='A'";
				break;
				case "M":
					$sqlRespUsuario = "SELECT
									  	h.hsuid as codigo,
									  	h.hsudsc as descricao
									FROM
									  	academico.hospitalunidade h
						              	inner join academico.usuarioresponsabilidade ur on ur.hsuid = h.hsuid and h.hsustatus = 'A'
									WHERE
								    	ur.usucpf = '%s' AND
								    	ur.pflcod = '%s' AND
								    	ur.rpustatus='A'";
					break;
			default:
				break;
		}
//		ver($sqlRespUsuario, d);
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		$respUsuario = $db->carregar($query);
//		ver($query);
		if (!$respUsuario || @count($respUsuario)<1) {
			//print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem associações a este Perfil.</font>";
		}
		else {
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
	<tr>
	  <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">
	    Total: (<?=@count($respUsuario)?>)
	  </td>
	</tr>
</table>
	<?
		}
	}
	$teste = $db->carregar("SELECT DISTINCT * FROM academico.usuarioresponsabilidade WHERE usucpf = '{$usucpf}' AND pflcod = {$pflcod} AND rpustatus = 'A'");
	if (!$teste) {
		print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem associações a este Perfil.</font>";
	}
}
$db->close();
exit();
?>