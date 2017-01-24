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

if(!$pflcod && !$usucpf) {
	?><font color="red">Requisição inválida</font><?php
	eixt();
}

$sqlResponsabilidadesPerfil = "SELECT tr.*
	FROM obras.tprperfil p
	INNER JOIN obras.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
	WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
	ORDER BY tr.tprdsc";

$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
$responsabilidadesPerfil = $db->carregar($query);
if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil)<1) {
	print "<font color='red'>Não foram encontrados registros</font>";
}
else {
	
	$arrPerfil = pegaPerfilgeral(); //pega todos os perfis do usuário
	$arrPerfil = retornaPflcodFilhos($arrPerfil); //retornar todos os perfis associados (seguranca.perfilpermissao)
	$perfilSuperUser = $db->testa_superuser(); //testa se o usuário é super usuário
	
	if(!$perfilSuperUser){

		//Verifica Permissão de Perfil (seguranca.perfilpermissao)
		$andPerfilPermissao = "AND perfil.pflcod in (".implode(",",$arrPerfil).") ";
		
		$sql = "SELECT 
					count(1)
				FROM 
					pg_namespace n, pg_class c
				WHERE 
					n.oid = c.relnamespace
				AND
					c.relkind = 'r'     -- no indices
				AND
					n.nspname not like 'pg\\_%' -- no catalogs
				AND
					n.nspname != 'information_schema' -- no information_schema
				AND
					n.nspname = '{$_SESSION['sisdiretorio']}'
				AND
					c.relname = 'usuarioresponsabilidade'";
			if($db->pegaUm($sql)){
				$sql = "select * from {$_SESSION['sisdiretorio']}.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A'";
				$arrDados = $db->carregar($sql);
				if($arrDados){
					foreach($arrDados as $dado){
						foreach($dado as $campo => $valor){
							if($campo != "rpuid" && $campo != "pflcod" && $campo != "usucpf" && $campo != "rpustatus" && $campo != "rpudata_inc"){
								if($valor){
									$arrCampo[$campo][] = $valor;
								}
							}
						}
					}
				}
			}
		
		if($arrCampo){
			foreach($arrCampo as $campo => $valor){
				if($campo && is_array($valor)){
					$arrWhere[] = "ur.$campo in ('".implode("','",$valor)."') ";
				}
			}
			$arrWhere[] = "ur.rpustatus = 'A'";
		}
	}
	
	
	foreach ($responsabilidadesPerfil as $rp) {
		//
		// monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (ação, programas, etc)
		$sqlRespUsuario = "";

		switch ($rp["tprsigla"]) {
			case "U": // Unidades 
				$aca_prg = "Unidades Associadas";
				$sqlRespUsuario = "SELECT 
									DISTINCT 
									e.entid AS codigo, 
									e.entnome ||' - '|| (SELECT
														 orgdesc 
														FROM
														 obras.orgao 
														WHERE orgid = CASE
																		WHEN funid = 12 THEN 1
																		WHEN funid = 11 OR funid = 14 THEN 2
																		WHEN funid = 16 OR funid = 44 THEN 5
																		WHEN funid = 118 THEN 6
																		ELSE 3
																	  END) || ' - ' || funid AS descricao, 
									ur.rpustatus AS status
								   FROM
								    obras.usuarioresponsabilidade ur 
								    INNER JOIN entidade.entidade e ON e.entid = ur.entid
								    INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
								   WHERE
								    ur.usucpf = '%s'
								    AND ur.pflcod = '%s'
                                    AND ur.rpustatus='A'
								    ". ($arrWhere ? ' AND '. implode(" AND ",$arrWhere): NULL);
				break;
			case "E": // Estados
				$aca_prg = "Estados Associados";
				$sqlRespUsuario = "SELECT DISTINCT 
									e.estuf AS codigo, 
									e.estdescricao AS descricao, 
									ur.rpustatus AS status
								   FROM 
								    obras.usuarioresponsabilidade ur 
									INNER JOIN territorios.estado e ON e.estuf = ur.estuf
									LEFT JOIN obras.orgao o ON o.orgid = ur.orgid
								   WHERE 
								    ur.usucpf = '%s' AND
								    ".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")." 
								    ur.pflcod = '%s' AND 
								    ur.rpustatus='A'";
				break;
			case "M": // Municípios
				$aca_prg = "Municípios Associados";
				$sqlRespUsuario = "
					select DISTINCT
						m.muncod as codigo,
						m.estuf || ' - ' || m.mundescricao as descricao,
						ur.rpustatus aS status
					from obras.usuarioresponsabilidade ur
						inner join territorios.municipio m on
							m.muncod = ur.muncod
					where
						ur.usucpf = '%s' and
						".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")."
						ur.pflcod = '%s' and
						ur.rpustatus = 'A'";
				break;
			case "O": // Órgão
				$aca_prg = "Órgão Associados";
				$sqlRespUsuario = "
					SELECT DISTINCT
						o.orgid AS codigo, o.orgdesc AS descricao
					FROM 
						obras.orgao AS o 
					INNER JOIN 
						obras.usuarioresponsabilidade AS ur 
					ON 
						o.orgid = ur.orgid
					WHERE 
						ur.usucpf = '%s' AND 
						".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")."
						ur.pflcod = '%s' AND ur.rpustatus='A'";
				break;
			case "B": // Obra
				$aca_prg = "Obras Associadas";
				/*$sqlRespUsuario = "
					SELECT DISTINCT
						oi.obrid AS codigo, 
						CASE WHEN (m.mundescricao is not null AND ed.estuf is not null AND o.orgdesc is not null) 
							THEN '| ' || orgdesc || ' | ' || oi.obrdesc || ' - ' || m.mundescricao || ' - ' || ed.estuf 
							ELSE oi.obrdesc 
						END as descricao
					FROM 
						obras.obrainfraestrutura oi 
					INNER JOIN 
						obras.usuarioresponsabilidade AS ur ON ur.obrid = oi.obrid
					INNER JOIN
						entidade.entidade 	 ee ON ee.entid = oi.entidunidade
					LEFT JOIN
						entidade.endereco 	 ed ON ed.endid = oi.endid
					LEFT JOIN 
						territorios.municipio m ON m.muncod = ed.muncod
					LEFT JOIN
						obras.orgao 		  o ON o.orgid = oi.orgid
					WHERE 
						ur.usucpf = '%s' AND 
						".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")."
						ur.pflcod = '%s' AND ur.rpustatus='A'";*/
				$sqlRespUsuario = "SELECT DISTINCT
										oi.obrid AS codigo,
				                        oi.obrdesc as descricao
									FROM 
										obras.usuarioresponsabilidade AS ur
										inner join obras.obrainfraestrutura oi ON oi.obrid = ur.obrid AND oi.obsstatus = 'A'
									WHERE 
										ur.usucpf = '%s' AND 
										".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")."
										ur.pflcod = '%s' AND ur.rpustatus='A'";
				break;
			default:
				break;
		}
		
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		$respUsuario = $db->carregar($query);
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
		<?php
			foreach ($respUsuario as $ru) {
		?>
	<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
      <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	  <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?if ($rp["tprsigla"]=='A'){?><a href="simec_er.php?modulo=principal/acao/cadacao&acao=C&acaid=<?=$ru["acaid"]?>&prgid=<?=$ru["prgid"]?>"><?=$ru["codigo"]?></a><?} else {print $ru["codigo"];}?></td>
	  <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?=$ru["descricao"]?></td>
	</tr>
		<?php
		}
		?>
	<tr>
	  <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">
	    Total: (<?=@count($respUsuario)?>)
	  </td>
	</tr>
</table>
	<?php
		}
	}
	$teste = $db->carregar("SELECT DISTINCT * FROM obras.usuarioresponsabilidade WHERE usucpf = '{$usucpf}' AND pflcod = {$pflcod} AND rpustatus = 'A'");
	if (!$teste) {
		print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem associações a este Perfil.</font>";
	}
}
$db->close();
exit();
?>