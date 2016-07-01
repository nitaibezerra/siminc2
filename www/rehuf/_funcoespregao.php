<?
function montaSubTitulo($texto, $return = false){
	$html = '<table cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" style="border-top: medium none; border-bottom: medium none;" class="tabela">
				<tbody>
					<tr>
						<td bgcolor="#e9e9e9" align="center"><label style="color: rgb(0, 0, 0); font-size: 13px;" class="TituloTela">' . $texto . '</label></td>
					</tr>
				</tbody>
			</table>';
	if ($return)
		return $html;
	else
		echo $html;
}

/*
 * Pregão MEC
*/

function pesquisaGrupoPregao($grunome){
	global $db;
	$sql = "SELECT 
			  ( '<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"cadastroItem('||gruid||','''||grunome||''');\" border=0 alt=\"Ir\" title=\"Alterar\"> ' ||
			      '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiGrupo('||gruid||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao,
			  grunome
			FROM 
			  rehuf.grupoitens
			WHERE grustatus = 'A'";
	if($grunome){
		$sql.= " AND lower(grunome) LIKE lower('%$grunome%')";
	}
	
	$sql.= " ORDER BY grunome";
	
	monta_titulo( '', 'Lista de Grupos' );
	$cabecalho = array("Opções", "Grupo");
	
	$db->monta_lista(iconv( "UTF-8", "ISO-8859-1", $sql), $cabecalho, 30, 4, 'N','Center','');
}

function excluiGrupoPregao($gruid){
	global $db;
	
	$sql = "UPDATE 
			  rehuf.grupoitens  
			SET 
			  grustatus = 'I',
			  gruins = NOW()
			 
			WHERE 
			  gruid = $gruid";
			
	$db->executar($sql);
	$res = $db->commit();
	
	if($res == "1"){
		echo $res;
	}else{
		echo "0";
	}
}

/*
 * itens de pregão
*/

function pesquisaItemPregao($itecatmat, $itedescricao, $iteapresentacao, $gruid){
	global $db;

	$sql = "SELECT 
			  ( '<center><a href=\"rehuf.php?modulo=pregao/cadastroItensPregao&acao=A&iteid='|| i.iteid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
			      '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiItem('''||i.iteid||''');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao,
			  i.iteid,
			  i.itecatmat,
			  '<b>'|| i.itedescricao || 
			  '</b><br>' || i.iteapresentacao as dados,
			  g.grunome,
			  il.itlabrev,
			  i.itecodsus
			FROM 
			  rehuf.item i 
			  inner join rehuf.itemgrupo ig 
			  on (i.iteid = ig.iteid) 
			  inner join rehuf.grupoitens g
			  on (ig.gruid = g.gruid) 
			  left join rehuf.itemlote il ON il.itlid = i.itlid
			WHERE i.itestatus = 'A'
			AND ig.itgstatus = 'A'
	 		AND g.grustatus = 'A'";
	if($itecatmat){
		$sql.= " AND i.itecatmat = '$itecatmat'";
	}
	if($itedescricao){
		$sql.= " AND lower(i.itedescricao) LIKE lower('%$itedescricao%')";
	}
	if($iteapresentacao){
		$sql.= " AND lower(i.iteapresentacao) LIKE lower('%$iteapresentacao%')";
	}
	if($gruid){
		$sql.= " AND g.gruid = '$gruid'";
	}
	
	$sql.= " ORDER BY il.itlabrev asc, i.itecatmat asc, i.iteid, g.grunome";
   	
	$arDados = $db->carregar(iconv( "UTF-8", "ISO-8859-1", $sql));
	
	if($arDados){
	
		$acao = "";
		$catmat = "";
		$iteidG = "";
		$grupo = array();
		$arGrupo = array();
		$nome = "";
		$itlabrev = "";
		$itecodsus = "";
		$registro = array();
		
		foreach ($arDados as $key => $value) {
			
			if($value['iteid'] != $iteidG){
				if($grupo){
					$array = Array("acao" => $acao,
						   "itecatmat" => $catmat,
						   "itedescicao" => $nome,
						   "grupo" => $grupo,
						   "itlabrev" => $itlabrev,
						   "itecodsus" => $itecodsus);
			
					array_push($registro, $array);
				}
				$acao = $value['acao'];
				$catmat = $value['itecatmat'];
				$iteidG = $value['iteid'];
				$grupo = $value['grunome'];
				$nome =  $value['dados'];
				$itlabrev =  $value['itlabrev'];
				$itecodsus = $value['itecodsus'];
			}else{
				$grupo = $grupo . "<br>" . $value['grunome'];
			}
		}
	
		$array = Array("acao" => $acao,
					   "itecatmat" => $catmat,
					   "itedescicao" => $nome,
					   "grupo" => $grupo,
					   "itlabrev" => $itlabrev,
					   "itecodsus" => $itecodsus);
		
		array_push($registro, $array);
		
		monta_titulo( '', 'Lista de Grupos' );
		$cabecalho = array("Opções", "CATMAT", "Descrição/Apresentação", "Grupo", '<span style="padding:10px;">Lote</span>', "Código SUS");
	
		$db->monta_lista_array($registro, $cabecalho, 30, 5, 'N','Center','');
	}else{
		$db->monta_lista(array(), $cabecalho, 30, 5, 'N','Center','');
	}
}

function excluiItemPregao($iteid){
	global $db;
	
	$sql = "UPDATE 
			  rehuf.item  
			SET 
			  itestatus = 'I'
			 
			WHERE 
			 iteid = $iteid";
			
	$db->executar($sql);
	
	/*$sql = "UPDATE 
			  rehuf.itemgrupo  
			SET 
			  itgstatus = 'I'
			WHERE 
			  iteid = $iteid";
			  
	$db->executar($sql);*/
	$res = $db->commit();
	
	if($res == "1"){
		echo $res;
	}else{
		echo "0";
	}
}

function insereItensPregao($itecatmat, $itedescricao, $iteapresentacao, $gruid, $itlid=false, $itecodsus=false){
	global $db;
	
	$grupo = explode(",", $gruid);
	
	$sql = "INSERT INTO 
			  rehuf.item(
			  itecatmat,
			  itedescricao,
			  iteapresentacao,
			  itestatus,
			  itlid,
			  itecodsus) 
			VALUES (
			  '$itecatmat',
			  upper('$itedescricao'),
			  upper('$iteapresentacao'),
			  'A',
			  ".(($itlid)?"'".$itlid."'":"NULL").",
			  ".(($itecodsus)?"'".$itecodsus."'":"NULL").")RETURNING iteid;";

	$iteid = $db->pegaUm(iconv( "UTF-8", "ISO-8859-1", $sql));
	
	foreach ($grupo as $gruid) {
		$sql = "INSERT INTO 
				  rehuf.itemgrupo(
				  gruid,
				  iteid,
				  itgstatus) 
				VALUES (
				  $gruid,
				  $iteid,
				  'A')";
		$db->executar($sql);
	}
	
	echo $db->commit();
	
}

function atualizaItensPregao($itgid, $iteid, $itecatmat, $itedescricao, $iteapresentacao, $gruid, $itlid=false, $itecodsus=false){
	global $db;
	
	$grupo = explode(",", $gruid);
	
	$ItemG = explode(",", $itgid);
	
	foreach($ItemG as $value){
		if(!in_array($value, $grupo)){
			$sql = "UPDATE 
					  rehuf.itemgrupo  
					SET 
					  itgstatus = 'I',
					  itgins = NOW()
					WHERE 
					  gruid = $value
					  AND iteid = $iteid";
			$db->executar($sql);
			$db->commit();
		}
	}

	$sql = "UPDATE 
			  rehuf.item  
			SET 
			  itecatmat = '$itecatmat',
			  iteins = NOW(),
			  itedescricao = upper('$itedescricao'),
			  iteapresentacao = upper('$iteapresentacao'),
			  itlid = ".(($itlid)?"'".$itlid."'":"NULL").",
			  itecodsus = ".(($itecodsus)?"'".$itecodsus."'":"NULL")."			 
			WHERE 
			  iteid = $iteid";
			  
	$db->executar($sql);

	foreach($ItemG as $value){
		foreach ($grupo as $gruid) {
			$sql = "SELECT
					  itgid
					FROM 
					  rehuf.itemgrupo
					WHERE gruid = $gruid
					  AND iteid = $iteid
					  AND itgstatus = 'I'";
			$retInativo = $db->pegaUm($sql);

			if($retInativo){
				$sql = "UPDATE 
						  rehuf.itemgrupo  
						SET 
						  itgstatus = 'A',
						  itgins = NOW() 				 
						WHERE 
						  gruid = $gruid
						  AND iteid = $iteid
						  AND itgstatus = 'I'";
				
				$db->executar($sql);
				//echo $sql."<br>";
			}else{
			
				$sql = "SELECT
						  itgid
						FROM 
						  rehuf.itemgrupo
						WHERE gruid = $gruid
						  AND iteid = $iteid";
				$ret = $db->pegaUm($sql);
	
				if(!$ret){
					$sql = "INSERT INTO 
							  rehuf.itemgrupo(
							  gruid,
							  iteid,
							  itgstatus) 
							VALUES (
							  $gruid,
							  $iteid,
							  'A')";
					$db->executar($sql);
					echo $db->commit();
				}else{
					if($value == $gruid){
						$sql = "UPDATE 
								  rehuf.itemgrupo  
								SET 
								  gruid = $gruid,
								  itgins = NOW(),
								  iteid = $iteid				 
								WHERE 
								  gruid = $value
								  AND iteid = $iteid";
						
						$db->executar($sql);
						$itg = $value;
					}
				}
			}
		}
	}
	echo $db->commit();
}

function pesquisaItem($itecatmat, $iteapresentação){
	global $db;
	$sql = "SELECT 
			  iteid,
			  itestatus
			FROM 
			  rehuf.item
			WHERE itecatmat = '$itecatmat'
			AND iteapresentacao = '$iteapresentação'";
	
	$retorno = $db->pegaLinha(iconv( "UTF-8", "ISO-8859-1", $sql));
	
	return $retorno;
}

/*
 * Manter pregão
*/

function pesquisarPregao($request){
	global $db;
	
	$request = is_array($request) ? $request : array();
	
	if($request['predatainicialpreenchimento'] != ""){
		$predatainicialpreenchimento = formata_data_sql($request['predatainicialpreenchimento']);
	}else{
		$predatainicialpregao = '';
	}
	
	if($request['predatafinalpreenchimento'] != ""){
		$predatafinalpreenchimento = formata_data_sql($request['predatafinalpreenchimento']);
	}else{
		$predatafinalpreenchimento = '';
	}	
	
	$sql = "SELECT 
			  ( '<center><a href=\"rehuf.php?modulo=pregao/cadastroPregao&acao=A&preid='|| preid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
			      '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiPregao('''||preid||''');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao,
			  precodigo,
			  preobjeto,
			  predatainicialpreenchimento || ' até ' ||
			  predatafinalpreenchimento,
			  (CASE 
				 WHEN rehuf.f_pregao_situacao_preenchimento(predatainicialpreenchimento, predatafinalpreenchimento) = 3 THEN 
				  '<center style=''color: #BCBCBC;font-weight:bold;''>Prenchimento não iniciado</center>'
				 WHEN rehuf.f_pregao_situacao_preenchimento(predatainicialpreenchimento, predatafinalpreenchimento) = 2 THEN 
				  '<center style=''color: #006600;font-weight:bold;''>Em preenchimento</center>'
				 WHEN rehuf.f_pregao_situacao_preenchimento(predatainicialpreenchimento, predatafinalpreenchimento) = 1 THEN 
				 '<center style=''color: #333399;font-weight:bold;''>Preenchimento concluído</center>'
				    END )  as opaid
			FROM 
			  rehuf.pregao
			WHERE prestatus = 'A'";
	
	if($request['precodigo']){
		$sql.=" AND precodigo = '{$request['precodigo']}'";
	}
	if($request['preobjeto']){
		$sql.=" AND lower(preobjeto) like lower('%".iconv( "UTF-8", "ISO-8859-1", $request["preobjeto"] )."%')";
	}
	if($request['predatainicialpreenchimento']){
		$sql.=" AND predatainicialpreenchimento BETWEEN '{$predatainicialpreenchimento}' AND '{$predatafinalpreenchimento}'";
	}
	
	if($request['radSituacaoPreenchimento'] != '' && $request['radSituacaoPreenchimento'] != '0' ){
		$sql.=" AND rehuf.f_pregao_situacao_preenchimento(predatainicialpreenchimento, predatafinalpreenchimento) = {$request['radSituacaoPreenchimento']}";
	}
	$sql.=" ORDER BY rehuf.f_pregao_situacao_preenchimento(predatainicialpreenchimento, predatafinalpreenchimento), predatafinalpreenchimento";

	monta_titulo( '', 'Lista de Pregões' );
	$cabecalho = array("Ações", "Código Pregão", "Objeto Pregão", "Período de Preenchimento", "Situação de Preenchimento");
	
	$db->monta_lista( $sql, $cabecalho, 30, 5, 'N','Center','');
}

function inserePregao($request){
	global $db;
	
	$retorno = verificaPregaoCodigo($request['precodigo']);
	if($retorno){
		echo "O código {$request['precodigo']} já existe em nossa base de dados. Favor informar outro código.";
	}else{
		if($request['predatainicialpregao'] != ""){
			$predatainicialpregao = "'".formata_data_sql($request['predatainicialpregao'])."'";
		}else{
			$predatainicialpregao = 'NULL';
		}
		
		if($request['predatafinalpregao'] != ""){
			$predatafinalpregao = "'".formata_data_sql($request['predatafinalpregao'])."'";
		}else{
			$predatafinalpregao = 'NULL';
		}
		
		$sql = "INSERT INTO 
				  rehuf.pregao(
				  preobjeto,
				  predatainicialpregao,
				  predatafinalpregao,
				  predatainicialpreenchimento,
				  predatafinalpreenchimento,
				  prestatus,
				  precodigo) 
				VALUES (
				  '{$request['preobjeto']}',".
				  $predatainicialpregao.",".
				  $predatafinalpregao.",'".
				  formata_data_sql($request['predatainicialpreenchimento'])."','".
				  formata_data_sql($request['predatafinalpreenchimento'])."',
				  'A',
			  	  ".(($request['precodigo'])?"'".$request['precodigo']."'":"NULL").") RETURNING preid;";
	
		$preid = $db->pegaUm( iconv( "UTF-8", "ISO-8859-1", $sql) );
		$db->commit();
		echo $preid;
	}
}

function atualizaPregao($request){
	global $db;
	
	if($request['predatainicialpregao'] != ""){
		$predatainicialpregao = "'".formata_data_sql($request['predatainicialpregao'])."'";
	}else{
		$predatainicialpregao = 'NULL';
	}
	
	if($request['predatafinalpregao'] != ""){
		$predatafinalpregao = "'".formata_data_sql($request['predatafinalpregao'])."'";
	}else{
		$predatafinalpregao = 'NULL';
	}
	
	$sql = "UPDATE 
			  rehuf.pregao  
			SET 
			  preobjeto = '{$request['preobjeto']}',
			  preins = NOW(),
			  predatainicialpregao = $predatainicialpregao,
			  predatafinalpregao = $predatafinalpregao,
			  predatainicialpreenchimento = '".formata_data_sql($request['predatainicialpreenchimento'])."',
			  predatafinalpreenchimento = '".formata_data_sql($request['predatafinalpreenchimento'])."',
			  precodigo = ".(($request['precodigo'])?"'".$request['precodigo']."'":"NULL")."			 
			WHERE 
			  preid = {$request['preid']}";
	
	$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
	echo $db->commit();
}

function verificaPregaoCodigo($precodigo){
	global $db;
	
	$sql = "SELECT 
			  preid
			FROM 
			  rehuf.pregao
			WHERE precodigo = '$precodigo'";

	return $db->pegaUm($sql);
}

function excluiPregao($preid){
	global $db;
	
	$sql = "UPDATE 
			  rehuf.pregao  
			SET
			  prestatus = 'I',
			  preins = NOW() 
			WHERE 
			  preid = $preid";
	
	$db->executar($sql);
	echo $db->commit();
}


function inserirItemGrupoPopUp($itgid, $preid){
	global $db;
	
	$arGrupo = explode(",", $itgid);
	$itgid = "";
	$igpid = "";
	$igpstatus = "";
	
	foreach ($arGrupo as $arGrupo) {
		
		$arSeparador = explode("_", $arGrupo);
		$itgid = $arSeparador[0];
		$igpid = $arSeparador[1];
		$igpstatus = $arSeparador[2];
		
		//Ainda não foi inserido, é necessário Insert
		if ($igpstatus == 'null'){
			$sql = "INSERT INTO 
				  rehuf.itemgrupopregao(
				  preid,
				  igpstatus,
				  itgid) 
				VALUES (
				  $preid,
				  'A',
				  $itgid)";
			
			$db->executar($sql);
		}
		// Já foi cadastrado, mas encontra-se inativo
		else if ($igpstatus == 'I'){
			$sql = "UPDATE 
					  rehuf.itemgrupopregao  
					SET 
					  igpstatus = 'A',
					  igpins = NOW() 					 
					WHERE 
					  igpid = $igpid";
			
			$db->executar($sql);
		}
		
			
	}
	echo $db->commit();
}

function pesquisaItemGrupoPregao($preid){
	global $db;
	
	$linha2 = "<label style=\"cursor: pointer\" onclick=\"AddItem();\"><img src=\"/imagens/gif_inclui.gif \" id=\"add\" border=0 alt=\"Ir\" title=\"Adicionar Item\">Adicionar Item</label>";
	
	$sql = "SELECT
			  ('<center><img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiItemGrupoPregao('''||igp.igpid||''');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>') as acao,
			  i.itecatmat,
			  '<b>'|| i.itedescricao || 
			  ' </b><br>' || i.iteapresentacao ||'' as dados,
			  g.grunome
			FROM 
			  rehuf.item i 
			  inner join rehuf.itemgrupo ig 
			  	on (i.iteid = ig.iteid) 
			  inner join rehuf.grupoitens g
			  	on (ig.gruid = g.gruid)
			  inner join rehuf.itemgrupopregao igp
    			on (ig.itgid = igp.itgid)
			WHERE i.itestatus = 'A'
			AND g.grustatus = 'A'
			AND ig.itgstatus = 'A'
			AND igp.igpstatus = 'A'
			AND igp.preid = $preid
			ORDER BY g.gruid, i.itlid, i.itecatmat, i.itedescricao, i.iteapresentacao";

			
	print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<td bgcolor="#e9e9e9" align="left" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
	
	monta_titulo( '', 'Lista de Itens do Pregão' );
	$cabecalho = array("Ação", "CAT MAT", "Descrição / Apresentação");
	$db->monta_lista_grupo($sql, $cabecalho, 60, 5, 'N','Center','','formItem', 'grunome');
	//$db->monta_lista( $sql, $cabecalho, 60, 5, 'N','Center','');
	//$arDados = $db->carregar($sql);
	
	$linha2 = "<label style=\"cursor: pointer\" onclick=\"AddItem();\"><img src=\"/imagens/gif_inclui.gif \" id=\"add\" border=0 alt=\"Ir\" title=\"Adicionar Item\">Adicionar Item</label>";
	print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<td bgcolor="#e9e9e9" align="left" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
}

function excluiItemGrupoPregao($igpid){
	global $db;
	
	$sql = "UPDATE 
			  rehuf.itemgrupopregao  
			SET 
			  igpstatus = 'I',
			  igpins = NOW() 					 
			WHERE 
			  igpid = $igpid";
	
	$db->executar($sql);
	echo $db->commit();	
}
/**
 * Função de lista dos pregões na visão dos hospitais
 * 
 * @author Alexandre Dourado
 * @return 
 * @param 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 10/07/2009
 */
function listaPregao($dados) {
	global $db;
	if($dados['meuspregoes']) {
		$filtro[] = "hp.hupid IS NOT NULL";		
	}
		
	$sql = "SELECT '<center><a href=\"?modulo=pregao/preenchimentoPregao&acao=A&preid=' || p.preid || '\"><img src=\"/imagens/alterar.gif\" border=\"0\"></a></center>' AS preencher,
				CASE WHEN precodigo IS NULL THEN preobjeto ELSE precodigo||' - '|| preobjeto END AS objeto,
				CASE WHEN current_date >= (predatafinalpreenchimento - INTERVAL '7 DAYS')::date THEN '<div style=\"color:red\">' || to_char(predatainicialpreenchimento, 'DD/MM/YYYY') || ' até ' || to_char(predatafinalpreenchimento, 'DD/MM/YYYY') || '</div>' 
					 ELSE '<div>' || to_char(predatainicialpreenchimento, 'DD/MM/YYYY') || ' até ' || to_char(predatafinalpreenchimento, 'DD/MM/YYYY') || '</div>' 
					 END AS periodo,	
				CASE WHEN hupid IS NULL THEN '<center>--</center>'
					ELSE '<center><img src=\"/imagens/valida1.gif\" border=\"0\"></center>'
					END AS participo
			FROM rehuf.pregao p
			LEFT JOIN rehuf.huparticipante hp ON hp.preid = p.preid AND entid = ".$_SESSION['rehuf_var']['entid']." AND hupstatus = '" . ATIVO . "'
			WHERE p.prestatus = '" . ATIVO . "' ".(($filtro)?" AND ".implode(" AND ", $filtro):"");
											   
	$cabecalho = array("Preencher", "Código - Objeto do Pregão", "Período de Preenchimento", "Pregões que Participo");
	$db->monta_lista($sql,$cabecalho,50,5,'N','center','');									   
}
/**
 * Função utilizada para listar os itens preenchidos
 * 
 * @author Alexandre Dourado
 * @return 
 * @param 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 10/07/2009
 */
function listaPreenchimentoItens($opcoes) {
	global $db, $permissoes;
	// se tiver problemas nas variaveis de sessão, imprimir o erro (ajax)
	if(validaVariaveisSistema()) {
		echo "<p align='center'>Problemas nas variáveis de sessão. <b><a href='?modulo=inicio&acao=C'>Clique aqui</a></b> e refaça os procedimentos.</p>";
		exit;	
	}
	// testando se o pregão existe
	if(!$_SESSION['rehuf_var']['preid']) {
		echo "<p align='center'>Problemas nas variáveis de sessão. <b><a href='?modulo=pregao/listaPregaoPreenchimento&acao=A'>Clique aqui</a></b> e refaça os procedimentos.</p>";
		exit;	
	}
	
	$sql = "SELECT
				itecatmat AS catmat,
				itedescricao AS descricao,
				iteapresentacao AS apresentacao,
				ippquantidadeanual AS quant,
				ippprecounitario AS pcunit,
				ippdescricaomarca AS marca,
				ippcnpjfornecedor AS cnpj,
				ippnomefornecedor AS nome_fornecedor,
				ippdatavencimentocontrato AS vigencia, 
				to_char(ippdatavencimentocontrato, 'DD/MM/YYYY') as vigenciaform,
				gi.gruid, 
				gi.grunome,
				ippid,
				ipp.igpid as igpid,
				il.itldesc,
				il.itlabrev,
				i.itecodsus,
                                i.iteitem
			FROM rehuf.huparticipante hp
			INNER JOIN rehuf.itemgrupopregao igp ON igp.preid = hp.preid AND igp.igpstatus = '" . ATIVO . "'
			INNER JOIN rehuf.itenspregaopreenchido ipp ON ipp.igpid = igp.igpid AND ipp.hupid = hp.hupid AND (ipp.ippstatus = '" . ATIVO . "' OR ipp.ippstatus = '".PREENCHIMENTO_PREGAO."')												 
			INNER JOIN rehuf.itemgrupo ig ON ig.itgid = igp.itgid AND ig.itgstatus = '" . ATIVO . "'
			INNER JOIN rehuf.item i ON i.iteid = ig.iteid AND i.itestatus = '" . ATIVO . "' 
			LEFT JOIN rehuf.itemlote il ON il.itlid = i.itlid 
			INNER JOIN rehuf.grupoitens gi ON gi.gruid = ig.gruid AND gi.grustatus = '" . ATIVO . "'
			WHERE hp.preid = {$_SESSION['rehuf_var']['preid']} AND 
				  hp.entid = {$_SESSION['rehuf_var']['entid']} AND 
				  hp.hupstatus = '" . ATIVO . "' ORDER BY il.itlabrev, i.iteitem, i.itecatmat,  i.itlid";
	$dados = $db->carregar($sql);
	
	if($dados[0]) {
		/*
		 * Cabeçalho do formulario
		 */
		$html .= '<form name="formulario" id="formulario" method="post" onsubmit="return validaFormularioItens();">
				  <input type="hidden" name="exec_function" value="gravaItem">';
		$html .= '<table class="tabela" cellspacing="0" cellpadding="2" border="0" align="center">';
		// verifica se o usuário possui perfil para alterar o pregão
		if($permissoes['gravarpregao']) {
			$html .= '<tr>
							<td class="SubTituloDireita">&nbsp;<a href="#" onclick="popupAssociaItemPregao(' . $preid . ')"><img src="/imagens/gif_inclui.gif" border="0"> Clique aqui para adicionar os itens de acordo com sua necessidade</a></td>
						</tr>';
		}
		$html .= '</table>';			
		
		$html .= '<table class="listagem" cellspacing="1" cellpadding="2" border="0" align="center" width="95%" style="background: #F5F5F5">
					<thead>
					<tr>
						<td class="title" colspan="6" style="background: #DCDCDC;"><center><b>Informações para este pregão</b></center></td>
						<td class="title" colspan="7"><center><b>Informações da última compra</b></center></td>
					</tr>
					<tr>
						<td class="title" style="background: #DCDCDC;"><center><b>CAT/MAT</b></center></td>
						<td class="title" style="background: #DCDCDC;"><center><b>Lote</b></center></td>
						<td class="title" style="background: #DCDCDC; width:50px;"><center><b>Item</b></center></td>
						<td class="title" style="background: #DCDCDC;"><center><b>Cod.SUS</b></center></td>
						<td class="title" style="background: #DCDCDC;"><center><b>Descrição/Apresentação</b></center></td>
						<td class="title" style="background: #DCDCDC;"><center><b>Quantidade</b></center></td>
						<td class="title"><center><b>Preço Unitário</b></center></td>
						<td class="title"><center><b>CNPJ do Fornecedor</b></center></td>
						<td class="title"><center><b>Nome do Fornecedor</b></center></td>
						<td class="title"><center><b>Fabricante/Marca</b></center></td>
						<td class="title"><center><b>Data Vigência</b></center></td>
						<td class="title"><center><b>Ação</b></center></td>
						<td style=width:10px;>&nbsp;</td>
					</tr>
					</thead>';
		$html .= '<tbody style="height:350px;overflow-y:scroll;overflow-x:hidden;" id="bodydata">'; 
		foreach ($dados AS $dado){
			if($dado['gruid'] != $gruidant) {
				$gruidant = $dado['gruid']; 
				$html .= '<tr><td colspan="6" style="background: #DCDCDC;"><strong>Grupo:</strong> '.$dado['grunome'].'</td><td colspan="6">&nbsp;</td></tr>';
			}
			/*
			 * Carregando os valores para o componentes campo_data
			 */
			$vigencia = 'vigencia'.$dado['ippid'];
			global $$vigencia;
			
			if($permissoes['gravarpregao']) {
				$$vigencia = $dado['vigencia'];
				
				unset($corlinha);
				if($dado['pcunit'] == $_REQUEST['flpreco'] && $dado['pcunit']) $corlinha = "#808080";
				
				$html .= '	<tr style="background: '.$corlinha.'">
								<td style="background: #FCFDDB">' . $dado['catmat'] . '</td>
								<td style="background: #FCFDDB">' . $dado['itlabrev'] . '</td>
								<td style="background: #FCFDDB; width:50px; text-align:center;">' . $dado['iteitem'] . '</td>
								<td style="background: #FCFDDB">' . $dado['itecodsus'] . '</td>
								<td title="' . $dado['descricao'] . $dado['apresentacao'] . '" style="background: #FCFDDB">' . limitaText($dado['descricao'] .' / '. $dado['apresentacao'], 50) . '</td>
								<td style="background: #FCFDDB">' . campo_texto('item[' . $dado['ippid'] . '][quantidade]', "S", "S", "Quantidade", 14, 15, "##############", "", 'right', '', 0, 'id="quantidade'.$dado['ippid'].'"', '', $dado['quant'], 'this.value=mascaraglobal(\'##############\',this.value);' ) . '</td>
								<td>' . campo_texto('item[' . $dado['ippid'] . '][precounit]', "N", "S", "Preço Unitário", 20, 19, "", "", 'right', '', 0, 'id="precounit'.$dado['ippid'].'"', 'this.value=formataPrecoPregao(this.value);', ((!is_null($dado['pcunit']))?mascaraglobal($dado['pcunit'], "###.###.###,####"):""),  'this.value=formataPrecoPregao(this.value);') . '</td>
								<td>' . campo_texto('item[' . $dado['ippid'] . '][cnpjfornecedor]', "N", "S", "CNPJ do fornecedor", 21, 20, "##.###.###/####-##", "", '', '', 0, 'id="cnpjfornecedor'.$dado['ippid'].'"', '', ((!is_null($dado['cnpj']))?mascaraglobal($dado['cnpj'], "##.###.###/####-##"):""), 'carregaNomeFornecedor(this);' ) . '</td>
								<td>' . campo_texto('item[' . $dado['ippid'] . '][nomefornecedor]', "N", "N", "Nome do Fornecedor", 20, 100, "", "", '', '', 0, 'id="nomefornecedor'.$dado['ippid'].'"', '', $dado['nome_fornecedor'] ) . '</td>
								<td>' . campo_texto('item[' . $dado['ippid'] . '][marcafornecedor]', "N", "S", "Fabricante/Marca", 20, 255, "", "", '', '', 0, 'id="marcafornecedor'.$dado['ippid'].'"', '', $dado['marca'] ) . '</td>
								<td>' . campo_data( 'vigencia'.$dado['ippid'], 'N', 'S', '', 'S' ) . '<input type="hidden" name="item[' . $dado['ippid'] . '][igpid]" value='.$dado['igpid'].'></td>
								<td><center><img src="/imagens/excluir.gif" style="cursor:pointer;" onclick="deletarItem('.$dado['ippid'].');"></center></td>
							</tr>';
			} else {
				$$vigencia = $dado['vigenciaform'];
				
				$html .= '	<tr>
								<td style="background: #FCFDDB">' . $dado['catmat'] . '</td>
								<td style="background: #FCFDDB">' . $dado['itldesc'] . '</td>
								<td style="background: #FCFDDB">' . $dado['itecodsus'] . '</td>
								<td title="' . $dado['descricao'] . $dado['apresentacao'] . '" style="background: #FCFDDB">' . limitaText($dado['descricao'] .' / '. $dado['apresentacao'], 50) . '</td>
								<td style="background:#FCFDDB;text-align:right;">' . $dado['quant'] . '</td>
								<td style="text-align:right;">' . ((!is_null($dado['pcunit']))?mascaraglobal($dado['pcunit'], "##.###.###,####"):"") . '</td>
								<td>' . ((!is_null($dado['cnpj']))?mascaraglobal($dado['cnpj'], "##.###.###/####-##"):"") . '</td>
								<td>' . $dado['nome_fornecedor'] . '</td>
								<td>' . $dado['marca'] . '</td>
								<td>' . $$vigencia .'</td>
								<td><center>&nbsp;</center></td>
							</tr>';
			
			}
		}
		$html .= '</tbody>';
		// verifica se o usuário possui perfil para alterar o pregão
		$html .= '<tfoot>';
		if($permissoes['gravarpregao']) {

			$html .= '<tr style="background: #DFDFDF;">
							<td colspan="13" class="SubTituloDireita">&nbsp;<a href="#" onclick="popupAssociaItemPregao();"><img src="/imagens/gif_inclui.gif" border="0"> Clique aqui para adicionar os itens de acordo com sua necessidade</a></td>
						</tr>';
		
			$html .= '<tr style="background: #DFDFDF;">
							<td colspan="13" align="center"><input type="submit" name="salvar" value="Salvar"> &nbsp; <input type="button" name="imprimir" value="Imprimir Extrato" onclick="imprimirExtrato();"></td>
						</tr>';
		} else {
		
			$html .= '<tr style="background: #DFDFDF;">
							<td colspan="13" align="center"><input type="button" name="imprimir" value="Imprimir Extrato" onclick="imprimirExtrato();"></td>
						</tr>';
		
		}
		$html .= '</tfoot>';		
		$html .= '</table>
				  </form>';			
	
	}else{
		// testando se existe hup
		$hupid = pegaPreenchimento($_SESSION['rehuf_var']['entid'], $_SESSION['rehuf_var']['preid']);
		if($hupid) {
			deletaHUParticipante($hupid);
		}
		
		$html .= '<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%">';
		// verifica se o usuário possui perfil para alterar o pregão
		if($permissoes['gravarpregao']) {
			$html .= '<tr>
							<td class="SubTituloDireita">&nbsp;<a href="#" onclick="popupAssociaItemPregao(' . $preid . ')"><img src="/imagens/gif_inclui.gif" border="0"> Clique aqui para adicionar os itens de acordo com sua necessidade</a></td>
						</tr>';
		}
		$html .= '<tr>
						<td><center style="color: red;">Não há itens preenchidos para este pregão</center></td>
					</tr>';
		// verifica se o usuário possui perfil para alterar o pregão
		if($permissoes['gravarpregao']) {
			$html .= '<tr>
							<td class="SubTituloDireita">&nbsp;<a href="#" onclick="popupAssociaItemPregao(' . $preid . ')"><img src="/imagens/gif_inclui.gif" border="0"> Clique aqui para adicionar os itens de acordo com sua necessidade</a></td>
						</tr>';
		}
		$html .= '</table>';			
	}	

	if ($opcoes['return'])
		return $html;
		
	echo $html;	

}
		
function carregaItemSelecionado ($hupid){
	global $db;
	$igpidArr = false;
	if(!empty($hupid)) {
		$sql = "SELECT igpid FROM rehuf.itenspregaopreenchido WHERE	hupid = {$hupid}";
		$igpidArr = $db->carregarColuna($sql);			
	}
	return $igpidArr;		
}

function carregaItem($preid) {
	global $db;
	if (empty($preid)){
		return false;			
	}
	$sql = "SELECT
				i.iteid,
				i.itecatmat,
				i.itedescricao,
				i.iteapresentacao,
				gi.gruid,
				gi.grunome,
				igp.igpid, 
				il.itlabrev,
				i.itecodsus,
                                i.iteitem
			FROM rehuf.itemgrupopregao igp
			INNER JOIN rehuf.itemgrupo ig ON ig.itgid = igp.itgid AND ig.itgstatus = '" . ATIVO . "'
			INNER JOIN rehuf.item i ON i.iteid = ig.iteid AND i.itestatus = '" . ATIVO . "' 
			LEFT JOIN rehuf.itemlote il ON il.itlid = i.itlid 
			INNER JOIN rehuf.grupoitens gi ON gi.gruid = ig.gruid AND gi.grustatus = '" . ATIVO . "'
			WHERE preid = {$preid} AND igpstatus = '" . ATIVO . "' ORDER BY il.itlabrev, i.iteitem, i.itecatmat, i.itlid";
	$dados = $db->carregar($sql);
	
	return $dados;
}

function pegaPreenchimento($entid, $preid){
	global $db;
	if (empty($entid) || empty($preid)){
		return false;			
	}
	
	$sql = "SELECT
				hupid
			FROM
				rehuf.huparticipante
			WHERE
				entid = {$entid}
				AND preid = {$preid}
				AND hupstatus = '" . ATIVO . "'";
	$hupid = $db->pegaUm($sql);
	
	return $hupid;					
}

function verificaPregao($preid){
	global $db;
	if(empty($preid))
		return false;
		
	$preid = $db->pegaUm("SELECT preid FROM rehuf.pregao WHERE preid={$preid}");

	return $preid;
}
/**
 * Função utilizada para cadastrar diversos itens preenchidos
 * 
 * @author Alexandre Dourado
 * @return void utilizado por AJAX
 * @param integer $dados[igpid] Array contendo ids do item com seu grupo
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 10/07/2009
 */
function cadastraTodosItem($dados) {
	global $db;
	// verifica se o hospital esta no pregão
	$hupid = pegaPreenchimento($_SESSION['rehuf_var']['entid'], $_SESSION['rehuf_var']['preid']);
	// se não estiver, inserir o hospital
	if(!$hupid) {
		// verificando as variaveis fundamentais
		if($_SESSION['rehuf_var']['entid'] && $_SESSION['rehuf_var']['preid']) {
			$hupid = $db->pegaUm("INSERT INTO rehuf.huparticipante(entid, preid, hupdataadesao, hupstatus)
   						  		  VALUES ('".$_SESSION['rehuf_var']['entid']."', '".$_SESSION['rehuf_var']['preid']."', NOW(), 'A') RETURNING hupid;");
		} else {
			exit;
		}
	}
	// verificando se existem igpids, depois varrer o array contendo ids
	if($dados['igpid']) {
		foreach($dados['igpid'] as $igpid) {
			$ippid = $db->pegaUm("SELECT ippid FROM rehuf.itenspregaopreenchido WHERE hupid='".$hupid."' AND igpid='".$igpid."'");
			// verificando se não existe o item preenchido
			if(!$ippid) {
				$sql = "INSERT INTO rehuf.itenspregaopreenchido(hupid, ippstatus, igpid)
			   			VALUES ('".$hupid."', 'N', '".$igpid."');";
				$db->executar($sql);
			}
		}
		$db->commit();
	}
}
/**
 * Função utilizada para cadastrar um Item preenchido
 * 
 * @author Alexandre Dourado
 * @return void utilizado por AJAX
 * @param integer $dados[igpid] ID do vinculo do item e grupoitem
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 10/07/2009
 */
function cadastraItem($dados) {
	global $db;
	// verifica se o hospital esta no pregão
	$hupid = pegaPreenchimento($_SESSION['rehuf_var']['entid'], $_SESSION['rehuf_var']['preid']);
	// se não estiver, inserir o hospital
	if(!$hupid) {
		// verificando as variaveis fundamentais
		if($_SESSION['rehuf_var']['entid'] && $_SESSION['rehuf_var']['preid']) {
			$hupid = $db->pegaUm("INSERT INTO rehuf.huparticipante(entid, preid, hupdataadesao, hupstatus)
   						  		  VALUES ('".$_SESSION['rehuf_var']['entid']."', '".$_SESSION['rehuf_var']['preid']."', NOW(), 'A') RETURNING hupid;");
		} else {
			exit;
		}
	}
	$ippid = $db->pegaUm("SELECT ippid FROM rehuf.itenspregaopreenchido WHERE hupid='".$hupid."' AND igpid='".$dados['igpid']."'");
	// testando se existe ippid, se não existir inserir
	if(!$ippid) {
		$sql = "INSERT INTO rehuf.itenspregaopreenchido(hupid, ippstatus, igpid) VALUES ('".$hupid."', 'N', '".$dados['igpid']."');";
		$db->executar($sql);
	}
	$db->commit();
}
/**
 * Função utilizada para atualizar os dados do itens preenchidos
 * 
 * @author Alexandre Dourado
 * @return javascriptcode Mensagem de confirmação e redirecionamento
 * @param integer $dados[item] Array contendo todas informações dos itens cadastrados
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 10/07/2009
 */
function gravaItem($dados) {
	global $db;
	// se existe itens a serem atualizados
	if($dados['item']) {
		// verifica se o hospital esta inserido em um pregão
		$hupid = pegaPreenchimento($_SESSION['rehuf_var']['entid'], $_SESSION['rehuf_var']['preid']);
		// se não tiver, insere o hospital no pregão
		if(!$hupid) {
			$act = "inserir";
			// se não tiver as variaveis de sessão inseridas, emite erro
			if($_SESSION['rehuf_var']['entid'] && $_SESSION['rehuf_var']['preid']) {
				$hupid = $db->pegaUm("INSERT INTO rehuf.huparticipante(entid, preid, hupdataadesao, hupstatus)
	 	  					  		  VALUES ('".$_SESSION['rehuf_var']['entid']."', '".$_SESSION['rehuf_var']['preid']."', NOW(), 'A') RETURNING hupid;");
			} else {
				echo "<script>alert('Problemas com váriaveis');window.location='?modulo=inicio&acao=C';</script>";
				exit;
			}
		}
		// varrendo os itens
		foreach($dados['item'] as $ippid => $itm) {
			// se tiver quantidade o status deve ser A, senão N
			if(round($itm['quantidade'])) $ippstatus = ", ippstatus='A'";
			else $ippstatus = ", ippstatus='N'";
			
			// caso o hospital tenha sido inserido nesta rotina, inserir os itens preenchidos
			if($act == "inserir") {
				$sql = "INSERT INTO rehuf.itenspregaopreenchido(hupid, ippquantidadeanual, ippprecounitario, ippdescricaomarca, ippcnpjfornecedor, ippdatavencimentocontrato, ippstatus, ippnomefornecedor, igpid)
	    				VALUES ('".$hupid."', ".(($itm['quantidade'])?"'".$itm['quantidade']."'":"NULL").", ".(($itm['precounit'])?"'".str_replace(array(".",","),array("","."),$itm['precounit'])."'":"NULL").", 
	    						 ".(($itm['marcafornecedor'])?"'".$itm['marcafornecedor']."'":"NULL").", ".(($itm['cnpjfornecedor'])?"'".str_replace(array(".","/","-"),array("","",""),$itm['cnpjfornecedor'])."'":"NULL").", 
	    						 ".(($dados['vigencia'.$ippid])?"'".formata_data_sql($dados['vigencia'.$ippid])."'":"NULL").", ".$ippstatus.", ".(($itm['nomefornecedor'])?"'".$itm['nomefornecedor']."'":"NULL").", ?);";
			} else {
			// senão atualizar os itens preenchidos
				$sql = "UPDATE rehuf.itenspregaopreenchido SET ippquantidadeanual=".(($itm['quantidade'])?"'".str_replace(array(","), array(""), $itm['quantidade'])."'":"NULL").",
															   ippins=NOW(),
															   ippprecounitario=".(($itm['precounit'] && is_numeric(str_replace(array(".",","),array("","."),$itm['precounit'])))?"'".str_replace(array(".",","),array("","."),$itm['precounit'])."'":"NULL").",
															   ippdescricaomarca=".(($itm['marcafornecedor'])?"'".$itm['marcafornecedor']."'":"NULL").",
															   ippcnpjfornecedor=".(($itm['cnpjfornecedor'])?"'".str_replace(array(".","/","-"),array("","",""),$itm['cnpjfornecedor'])."'":"NULL").",
															   ippdatavencimentocontrato=".(($dados['vigencia'.$ippid])?"'".formata_data_sql($dados['vigencia'.$ippid])."'":"NULL").",
															   ippnomefornecedor=".(($itm['nomefornecedor'])?"'".$itm['nomefornecedor']."'":"NULL")."
															   ".$ippstatus." WHERE ippid='".$ippid."'";
			}
			$db->executar($sql);
		}
		$db->commit();
	}
	echo "<script>
			alert('Dados atualizados com sucesso');
			window.location='?modulo=pregao/preenchimentoPregao&acao=A&preid=".$_SESSION['rehuf_var']['preid']."';
		  </script>";
	exit;
}
/**
 * Função utilizada para deletar hospital participante do pregão.
 * 
 * @author Alexandre Dourado
 * @return void
 * @param integer $hupid ID do hospital participante de um pregão
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 10/07/2009
 */
function deletaHUParticipante($hupid) {
	global $db;
	$db->executar("DELETE FROM rehuf.itenspregaopreenchido WHERE hupid='".$hupid."'");
	$db->executar("DELETE FROM rehuf.huparticipante WHERE hupid='".$hupid."'");
	$db->commit();
}
/**
 * Função utilizada para deletar um item preenchido no pregão.
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[ippid] ID do item preenchido
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 10/07/2009
 */
function deletaItem($dados) {
	global $db;
	$db->executar("DELETE FROM rehuf.itenspregaopreenchido WHERE ippid='".$dados['ippid']."'");
	$db->commit();
}

function limitaText($txt, $maxText=100){
	if (empty($txt))
		return;
		
	if (strlen($txt) > $maxText){	
		$txt = substr($txt, 0, $maxText);
		$txt .= '...';
	}
	return $txt;
}
/**
 * Função que monta o cabeçalho do pregão selecionado
 * @author Fernando A. Bagno da Silva
 * @since 22/07/2009
 * @param integer $preid
 * @return mixed
 */
function rehuf_cabecalho_pregao( $preid ){
	
	global $db;
	
	$sql = "SELECT 
				CASE WHEN precodigo <> '' THEN precodigo ELSE 'Não Informado' END as codigo,
				preobjeto,
				to_char(predatainicialpreenchimento, 'DD/MM/YYYY') as inicialpreenchimento,
				to_char(predatafinalpreenchimento, 'DD/MM/YYYY') as finalpreenchimento,
				to_char(predatainicialpregao, 'DD/MM/YYYY') as inicialpregao,
				to_char(predatafinalpregao, 'DD/MM/YYYY') as finalpregao
			FROM 
				rehuf.pregao 
			WHERE 
				preid = {$preid}";
	
	$pregao = $db->pegaLinha( $sql );
	
	$periodopreenchimento = !empty($pregao['inicialpreenchimento']) && !empty($pregao['finalpreenchimento'])  ?
							'de ' . $pregao['inicialpreenchimento'] . ' até ' . $pregao['finalpreenchimento'] : 'Não Informado'; 
	
	$vigenciapregao = !empty($pregao['inicialpregao']) && !empty($pregao['finalpregao'])  ?
					  'de ' . $pregao['inicialpregao'] . ' até ' . $pregao['finalpregao'] : 'Não Informado';
	
	// html com os dados do pregão
	$cabecalho = '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">'
			   . '	<tr><td colspan="2" class="SubTituloCentro">Dados do Pregão</td></tr>'
			   . '	<tr>'
			   . '		<td class="subTituloDireita" width="15%;">Código do Pregão:</td>'
			   . '		<td>' . $pregao['codigo'] . '</td>'
			   . '	</tr>'
			   . '	<tr>'
			   . '		<td class="subTituloDireita">Objeto do Pregão:</td>'
			   . '		<td>' . $pregao['preobjeto'] . '</td>'
			   . '	</tr>'
			   . '	<tr>'
			   . '		<td class="subTituloDireita">Período de Preenchimento:</td>'
			   . '		<td>' . $periodopreenchimento . '</td>'
			   . '	</tr>'
			   . '	<tr>'
			   . '		<td class="subTituloDireita">Vigência do Pregão:</td>'
			   . '		<td>' . $vigenciapregao . '</td>'
			   . '	</tr>'
			   . '</table>';
	
	return $cabecalho;
			   
}

function rehuf_lista_hospitais_participantes_pregao($preid, $filtro=false ){
	
	global $db;
	
	if($filtro['preco']) {
		$filtroG[] = "it.ippprecounitario='".str_replace(array(".",","),array("","."),$filtro['preco'])."'";
	}

	$sql = "SELECT 
				ent.entnome, ena.entsig, 
				mundescricao || ' / ' || ende.estuf as mun_estado,
				to_char(hupdataadesao, 'DD/MM/YYYY') as adesao,
				'<center><img src=\"../imagens/consultar.gif\"/ style=\"cursor:pointer;\" onclick=\"rehuf_visualiza_itens({$preid}, '|| ent.entid || '".(($filtro['preco'])?",\'".str_replace(array(".",","),array("","."),$filtro['preco'])."\'":"").");\"></center>' as visualizaritens  
			FROM 
				entidade.entidade ent
			INNER JOIN
				rehuf.huparticipante rh ON rh.entid = ent.entid
			LEFT JOIN 
				entidade.funcaoentidade fen ON fen.entid = ent.entid
			LEFT JOIN 
				entidade.funentassoc fue ON fue.fueid = fen.fueid
			LEFT JOIN 
				entidade.entidade ena ON ena.entid = fue.entid
			LEFT JOIN 
				entidade.endereco ende ON ende.entid = ent.entid 
			LEFT JOIN 
				territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf
			LEFT JOIN 
				rehuf.itenspregaopreenchido it ON it.hupid = rh.hupid
			WHERE fen.funid = ".HOSPITALUNIV." AND 
				preid = {$preid} ".(($filtroG)?"AND ".implode(" AND ",$filtroG):"")."
			GROUP BY 
				ent.entnome, ena.entsig, mundescricao, ende.estuf, hupdataadesao, ent.entid
			ORDER BY 
				ena.entsig, ent.entnome";
				
	$cabecalho = array("Hospital", "IFES", "Município/UF", "Data de Adesão", "Visualizar Itens");
	$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'N', '100%');
	
}

function relat_preco_referencia_pregao( $preid ){
	global $db;
	
	$sql = "SELECT 
	 			i.iteid,
				i.itecatmat,
			    '<b>'|| i.itedescricao || 
			    '</b><br>' || i.iteapresentacao as dados,
			    ipp.ippquantidadeanual,
			    ipp.ippprecounitario,
			    igp.igpprecoreferencia,
			    ipp.igpid,
			    il.itldesc,
			    i.itecodsus
			FROM rehuf.item i INNER JOIN rehuf.itemgrupo ig
			  ON (i.iteid = ig.iteid) INNER JOIN rehuf.grupoitens gi
			  ON (ig.gruid = gi.gruid) INNER JOIN rehuf.itemgrupopregao igp
			  ON (ig.itgid = igp.itgid) INNER JOIN rehuf.pregao p
			  ON (igp.preid = p.preid) INNER JOIN rehuf.itenspregaopreenchido ipp
			  ON (igp.igpid = ipp.igpid)
			  LEFT JOIN rehuf.itemlote il ON il.itlid = i.itlid 
			WHERE ipp.ippstatus = 'A' 
			  AND p.preid = $preid
			ORDER BY i.itecatmat, ipp.igpid";
	
	$cabecalho = array("&nbsp;","CATMAT","Cod SUS","Lote", "Descrição/Apresentação", "QtdeTotal", "Mínimo", "Maximo", "Mediana", "Média", "Desvio Padrão", "Preço de Referêcnia");
	
	$arDados = $db->carregar($sql);	

	$arQtdeAnual = array();
	$arPrecoUnit = array();
	$arPrecoUnit1 = array();

	$estQtde = new Estatistica();
	$estUnit = new Estatistica();

	$estUnit->precisao = 40;
	?>
	<form name="formulario" id="formulario" action="" method="post">

	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<?foreach ($cabecalho as $cab) {?>				
				<td align="Center" valign="top" class="title"
					style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"
					title="Ordenar por Selecionar"><strong><?=$cab ?></strong>
			<?}?>		
		</tr>
	</thead>
		<?
		$igpidAnt = "";
		$chave = 0;
		if($arDados){
			foreach ($arDados as $key => $value) {
					
				if($igpidAnt != $value['igpid']){
					
					$chave % 2 ? $cor = "#f7f7f7" : $cor = "";

					$arQtdeAnual = carregaArrayQuant($arDados, $value['igpid']);
					$arPrecoUnit  = carregaArrayPreco($arDados, $value['igpid']);
					
					$estQtde->setArray($arQtdeAnual);
					$estUnit->setArray($arPrecoUnit);			

			?>
				<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
					<td><img src="../imagens/mais.gif" onclick="detalhesPrecoReferencia(this, '<?=$value['iteid'] ?>');" style="cursor:pointer;" title="mais"></td>
					<td><?=$value['itecatmat'] ?></td>
					<td><?=$value['itecodsus'] ?></td>
					<td><?=$value['itldesc'] ?></td>
					<td><?=$value['dados'] ?></td>
					<td align="right" title="QtdeTotal" style="color: rgb(0, 102, 204);"><?=$estQtde->getSomaDosElementos(); //Qauntidade total?></td>
					<td align="right" title="QtdeTotal" style="color: rgb(0, 102, 204);"><?=formata_valor($estUnit->getMin(), 4);?></td>
					<td align="right" title="QtdeTotal" style="color: rgb(0, 102, 204);"><?=formata_valor($estUnit->getMax(), 4);?></td>
					<td align="right" title="QtdeTotal" style="color: rgb(0, 102, 204);"><?=formata_valor($estUnit->getMediana(), 4);?></td>
					<td align="right" title="QtdeTotal" style="color: rgb(0, 102, 204);"><?=formata_valor($estUnit->getMediaAritmetica(), 4);?></td>
					<td align="right" title="QtdeTotal" style="color: rgb(0, 102, 204);"><?=formata_valor($estUnit->getDesvioPadrao(),4);?></td>
					<td align="center"><input type="text" value="<?=$value['igpprecoreferencia'] ? formata_valor($value['igpprecoreferencia'], 4) : ''; ?>" name="igpprecoreferencia" id="igpprecoreferencia" onKeyUp="this.value=formataPrecoPregao(this.value);">
								       <input type="hidden" name="ippid" id="ippid" value="<?=$value['igpid']; ?>"></td>
				</tr>
		  <?	
		  			$igpidAnt = $value['igpid'];
		  			$chave++;
				}
			}
		?>
	<?}
	else{?>
			<tr>
				<td align="center" style="color: rgb(204, 0, 0);" colspan="<?=count($cabecalho); ?>">Não foram encontrados Registros.</td>
			</tr>
		</table>
		</form>
  <?}
  	if($chave > 0){
  ?>
	<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%">
		<tbody>
			<tr bgcolor="#ffffff">
				<td><b>Total de Registros: <?=$chave; ?></b></td>			
			</tr>
		</tbody>
			<tr>
				<th align="center" colspan="2"><input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="atualizaPrecoReferencia();">
				<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="limparDados();"></th>
			</tr>
	</table>
<?
  	}
}

function carregaArrayQuant($array, $id){
	$arQtdeAnual = array();
	
	foreach ($array as $key => $array) {
		if($id == $array['igpid']){
			if($array['ippquantidadeanual'])
				$arQtdeAnual[] = $array['ippquantidadeanual'];
		}
	}
	return $arQtdeAnual;
}

function carregaArrayPreco($array, $id){
	$arPrecoUnit = array();
	
	foreach ($array as $key => $array) {
		if($id == $array['igpid']){
			if($array['ippprecounitario'])
				$arPrecoUnit[] = $array['ippprecounitario'];
		}
	}	
	return $arPrecoUnit;
}

function atualizaPrecoReferencia($valor){
	global $db;
	
	$valor = explode('|', $valor);

	foreach ($valor as $valor) {
		
		$igpprecoreferencia = substr($valor, 0, strpos($valor, '_'));
		$igpprecoreferencia = str_replace(".","",substr($igpprecoreferencia,0,strrpos($igpprecoreferencia, '.'))).substr($igpprecoreferencia,strrpos($igpprecoreferencia, '.'));
		$igpid = substr($valor, strpos($valor, '_') +1, strlen($valor) );
		
		$sql = "UPDATE 
				  rehuf.itemgrupopregao  
				SET 
				  igpprecoreferencia = ".($igpprecoreferencia ? "'".$igpprecoreferencia."'" : "NULL").",
				  igpins = NOW() 
			 
				WHERE 
				  igpid = $igpid";
		$db->executar($sql);

	}
	echo $db->commit();	
}

function carregaDetalhesPrecoReferencia($dados) {
	global $db;
	$sql = "SELECT 
				ent.entnome, ena.entsig, 
				mundescricao || ' / ' || ende.estuf as mun_estado,
				to_char(hupdataadesao, 'DD/MM/YYYY') as adesao,
				it.ippquantidadeanual,
				it.ippprecounitario,
				it.ippdescricaomarca,
				'<center><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"rehuf_visualiza_itens(\'".$_SESSION['rehuf']['preid']."\', \''|| ent.entid || '\', \''|| COALESCE(cast(it.ippprecounitario as varchar),'') ||'\');\"></center>' as visualizaritens  
			FROM 
				entidade.entidade ent
			INNER JOIN
				rehuf.huparticipante rh ON rh.entid = ent.entid
			LEFT JOIN 
				entidade.funcaoentidade fen ON fen.entid = ent.entid
			LEFT JOIN 
				entidade.funentassoc fue ON fue.fueid = fen.fueid
			LEFT JOIN 
				entidade.entidade ena ON ena.entid = fue.entid
			LEFT JOIN 
				entidade.endereco ende ON ende.entid = ent.entid
			LEFT JOIN 
				territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
			LEFT JOIN 
				rehuf.itenspregaopreenchido it ON it.hupid = rh.hupid 
			LEFT JOIN 
				rehuf.itemgrupopregao itg ON itg.igpid = it.igpid 
			LEFT JOIN 
				rehuf.itemgrupo ig ON ig.itgid = itg.itgid
			WHERE fen.funid = ".HOSPITALUNIV." AND 
				itg.preid = '".$_SESSION['rehuf']['preid']."' AND ig.iteid='".$dados['iteid']."' AND it.ippstatus = 'A'
			ORDER BY 
				ent.entnome";
	$cabecalho = array("Hospital", "IFES", "Munícipio/UF", "Data de Adesão", "Quantidade", "Preço Unitário", "Marca", "Visualizar Itens");
	$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'N', '100%');
}
?>