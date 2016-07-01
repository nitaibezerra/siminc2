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
	exit();
}

$sqlResponsabilidadesPerfil = " SELECT tr.*
								FROM 
									obras2.tprperfil p
								INNER JOIN obras2.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
								WHERE 
									tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
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
			case "P": // Empresa 
				$aca_prg = "Empresas Associadas";
				$sqlRespUsuario = "	SELECT DISTINCT 
										e.entid AS codigo, 
										e.entnome AS descricao, 
										ur.rpustatus AS status
								   	FROM 
								    	obras2.usuarioresponsabilidade ur 
									INNER JOIN entidade.entidade e ON e.entid = ur.entid
								   	WHERE 
									    ur.usucpf = '%s' AND
									    ".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")." 
									    ur.pflcod = '%s' AND 
									    ur.rpustatus='A'
									ORDER BY
										2";
				break;
			case "U": // Unidades 
				$aca_prg = "Unidades Associadas";
				$sqlRespUsuario = "SELECT DISTINCT 
										e.entid AS codigo, 
										e.entnome ||' - '||(SELECT
														 		orgdesc 
															FROM
														 		obras2.orgao 
															WHERE orgid = CASE
																			WHEN funid = 12 THEN 1
																			WHEN funid = 11 OR funid = 14 THEN 2
																			WHEN funid = 16 THEN 5
																		  	ELSE 3
																	  	  END) || ' ' || funid AS descricao, 
										ur.rpustatus AS status
								   FROM
								   		obras2.usuarioresponsabilidade ur 
								   INNER JOIN entidade.entidade 		 e ON e.entid = ur.entid
								   INNER JOIN entidade.funcaoentidade 	ef ON ef.entid = e.entid
								   WHERE
									    ur.usucpf = '%s' AND 
									    ur.pflcod = '%s' AND 
									    ur.rpustatus='A' AND 
									    ".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")." 
									    ef.funid in (1,3, 6, 7, 11, 12, 14, 16, 34, 43, 44)";
				break;
			case "E": // Estados
				$aca_prg = "Estados Associados";
				$sqlRespUsuario = "	SELECT DISTINCT 
										e.estuf AS codigo, 
										e.estdescricao AS descricao, 
										ur.rpustatus AS status
								   	FROM 
								    	obras2.usuarioresponsabilidade ur 
									INNER JOIN territorios.estado e ON e.estuf = ur.estuf
									LEFT JOIN obras2.orgao o ON o.orgid = ur.orgid
								   	WHERE 
									    ur.usucpf = '%s' AND
									    ".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")." 
									    ur.pflcod = '%s' AND 
									    ur.rpustatus='A'
									ORDER BY
										1";
				break;
// 			case "M": // Municípios
// 				$aca_prg = "Municípios Associados";
// 				$sqlRespUsuario = "
// 					SELECT DISTINCT
// 						m.muncod as codigo,
// 						m.estuf || ' - ' || m.mundescricao as descricao,
// 						ur.rpustatus aS status
// 					FROM 
// 						obras2.usuarioresponsabilidade ur
// 					INNER JOIN territorios.municipio m ON m.muncod = ur.muncod
// 					WHERE
// 						ur.usucpf = '%s' and
// 						".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")."
// 						ur.pflcod = '%s' and
// 						ur.rpustatus = 'A'";
// 				break;
			case "O": // Órgão
				$aca_prg = "Órgão Associados";
				$sqlRespUsuario = "
					SELECT DISTINCT
						o.orgid AS codigo, o.orgdesc AS descricao
					FROM 
						obras.orgao AS o 
					INNER JOIN 
						obras2.usuarioresponsabilidade AS ur 
					ON 
						o.orgid = ur.orgid
					WHERE 
						ur.usucpf = '%s' AND 
						".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")."
						ur.pflcod = '%s' AND ur.rpustatus='A'";
				break;
			case "B": // Empreendimento 
				$aca_prg = "Obras Associadas";
				$sqlRespUsuario = "SELECT DISTINCT
										e.empid AS codigo,
				                        e.empdsc as descricao
									FROM 
										obras2.usuarioresponsabilidade AS ur
									INNER JOIN obras2.empreendimento e ON e.empid = ur.empid AND 
																		  e.empstatus = 'A'
									WHERE 
										ur.usucpf = '%s' AND 
										".($arrWhere ? implode(" AND ",$arrWhere)." AND " : "")."
										ur.pflcod = '%s' AND ur.rpustatus='A'";
				break;
			case "M": // Empresa MI 
				$aca_prg = "Empresas MI Associadas";
				
				$sqlRespUsuario = "SELECT DISTINCT
										e.emiid as codigo,
										e.emidsc as descricao
									FROM
										obras2.usuarioresponsabilidade ur
									INNER JOIN obras2.empresami e ON e.emiid = ur.emiid AND e.emistatus = 'A'
									WHERE 
										ur.usucpf = '%s' AND 
										ur.rpustatus = 'A' AND
										ur.pflcod = '%s'";
				break;
			default:
				break;
		}
		
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		$respUsuario = $db->carregar($query);
		if (!$respUsuario || @count($respUsuario)<1) {
			//print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem associações a este Perfil.</font>";
		}else{
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
	$teste = $db->carregar("SELECT DISTINCT * FROM obras2.usuarioresponsabilidade WHERE usucpf = '{$usucpf}' AND pflcod = {$pflcod} AND rpustatus = 'A'");
	if (!$teste) {
		print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem associações a este Perfil.</font>";
	}
}
$db->close();
exit();
?>