<?php

// -------------------- CONTROLE DE LISTAGEM E BUSCA DE OBRAS --------------------

/**
 * Função que lista as obras da unidade
 *
 * @author Fernando Araújo Bagno da Silva
 * @param string $somenteLeitura
 * @return mixed
 *  
 */
function obras_listaObras($somenteLeitura){

	global $db;

	if ($somenteLeitura == "N"){
        $excluir = '<img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\">';
    } else {
        $excluir = '<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"javascript:Excluir(\\\'?modulo=inicio&acao=A&requisicao=exclusao\\\', \' || oi.obrid || \');\">';
    }

    // Se o usuário não for Super Usuário, serão exibidas apenas obras de sua unidade.
	$where = " WHERE ";
	$criteria = "";

	if ($_REQUEST["orgid"])
        $criteria .= ' and  oi.orgid'." = ".$_REQUEST["orgid"];

	if ($_REQUEST["stoid"])
        $criteria .= ' and sto.stoid'." = ".$_REQUEST["stoid"];

    if ($_REQUEST["entid"])
        $criteria .= ' and et.entid'." = ".$_REQUEST["entid"];

	if ($_REQUEST["obrdesc"])
        $criteria .= " and UPPER(oi.obrdesc) LIKE UPPER('%".$_REQUEST["obrdesc"]."%') ";	

	if ($criteria){
        $criteria = $where." oi.obsstatus = 'A' ".$criteria;
	} else {
        $criteria = "WHERE oi.obsstatus = 'A'";
	}

	// Verifica se existe foto cadastrada na obra.
	if ($_REQUEST["foto"]) {
		if ($_REQUEST["foto"] == "sim") {
			$criteria .= " and (case when ao.obrid is null then false else true end) = 't' ";
		} elseif ($_REQUEST["foto"] == "nao") {
			$criteria .= " and (case when ao.obrid is null then false else true end) = 'f' ";
		}
	}

	// Verifica se existe vistoria cadastrada na obra.
	if ($_REQUEST["vistoria"]) {
		if ($_REQUEST["vistoria"] == "sim") {
			$criteria .= " and (case when s.obrid is null then false else true end) = 't' ";
		} elseif($_REQUEST["vistoria"] == "nao") {
			$criteria .= " and (case when s.obrid is null then false else true end) = 'f' ";
		}
	}

    $sql = "
	SELECT DISTINCT
		'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro&acao=U\',' || oi.obrid || ');\">' || 
		'{$excluir} </center>' as acao,
		case when s.obrid is null then '' else '<img src=\"/imagens/anexo.gif\" border=0 title=\"Ver documentos\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\documentos&acao=A\',' || oi.obrid || ');\">' end as documento,
		case when ao.obrid is null then '' else '<img src=\"/imagens/cam_foto.gif\" border=0 title=\"Galeria de fotos\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\album&acao=A\',' || oi.obrid || ');\">' end as foto,
		case when r.obrid is null then '' else '<img src=\"/imagens/restricao.png\" border=0 title=\"Restrição\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal/restricao&acao=A\',' || oi.obrid || ');\">' end as restricao,
        '<a style=\"margin: 0 -20px 0 20px; text-transform:capitalize;\" href=\"#\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro&acao=U\',' || oi.obrid || ');\">' || oi.obrdesc || '</a>' as nome_obra,
		to_char(oi.obrdtinicio,'DD/MM/YYYY') as inicio,
		to_char(oi.obrdttermino,'DD/MM/YYYY') as final,
		(select ROUND(SUM(icopercexecutado),2) as total from obras.itenscomposicaoobra WHERE obrid = oi.obrid ) || '%' as percentual,
		sto.stodesc as situacao
	FROM 
		obras.obrainfraestrutura oi INNER JOIN entidade.entidade et 
		ON oi.entidunidade = et.entid 
		INNER JOIN obras.situacaoobra sto 
		ON oi.stoid = sto.stoid 
		LEFT JOIN obras.arquivosobra ao ON ao.obrid = oi.obrid and ao.aqostatus = 'A' and ao.tpaid = 21
		LEFT JOIN obras.supervisao s ON s.obrid = oi.obrid and s.supstatus = 'A'
		LEFT JOIN obras.restricaoobra r ON r.obrid = oi.obrid and r.rststatus = 'A' 
		INNER JOIN obras.orgao org 
		ON oi.orgid = org.orgid " . $criteria. "
        and entnome <> '' and oi.entidunidade = " . $_REQUEST['carga'] . "
		group BY org.orgdesc, oi.obrid, oi.obrdesc, et.entnome, oi.obrdtinicio, oi.obrdttermino, 
				sto.stodesc, s.obrid, r.obrid, ao.obrid";
    
   		$cabecalho = array( "Ação", "A", "F", "R", "Nome da Obra", "Data de Início", "Data de Término", "(%) Executado", "Situação da Obra" );
		$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%');

	die();

}

/**
 * Função que realiza a busca por obras cadastradas
 * 
 * @author Fernando Araújo Bagno da Silva
 * @param array $dados
 * @return string
 * 
 */
function obras_pesquisa($dados){

	$where = " WHERE ";
	$criteria = "";
	
	if ($dados["orgid"])
		$criteria .= ' and  oi.orgid'." = ".$dados["orgid"];
	
	if ($dados["stoid"])
		$criteria .= ' and sto.stoid'." = ".$dados["stoid"];
	
	if ($dados["entid"])
		$criteria .= ' and et.entid'." = ".$dados["entid"];
	
	if ($dados["obrdesc"])
		$criteria .= " and UPPER(oi.obrdesc) LIKE UPPER('%".$dados["obrdesc"]."%') ";	
	
	if ($dados["convenio"]){
		$criteria .= " and frrconvnum = '{$dados["convenio"]}' ";
	}
	
	if($criteria){
		$criteria = $where." oi.obsstatus = 'A' ".$criteria;
	}else{
		$criteria = "WHERE oi.obsstatus = 'A'";
	}	
	
	if($dados["foto"]) {
		if($dados["foto"] == "sim") {
			$criteria .= " and (case when ao.obrid is null then false else true end) = 't' ";
		}
		else if($dados["foto"] == "nao") {
			$criteria .= " and (case when ao.obrid is null then false else true end) = 'f' ";
		}
	}
	
	if($dados["vistoria"]) {
		if($dados["vistoria"] == "sim") {
			$criteria .= " and (case when s.obrid is null then false else true end) = 't' ";
		}
		else if($dados["vistoria"] == "nao") {
			$criteria .= " and (case when s.obrid is null then false else true end) = 'f' ";
		}
	}
	
	return $criteria;
	
}

/**
 * Função que verifica se o usuário logado possui perfil de consulta
 *
 * @author Fernando Araújo Bagno da Silva
 * @return boolean
 * 
 */
function obras_possuiPerfilConsulta(){

	if( possuiPerfil(160) || possuiPerfil(155) || 
		possuiPerfil(166) || possuiPerfil(165) || 
		possuiPerfil(167) || possuiPerfil(162) || 
		possuiPerfil(158) ){
			
		$consulta = false;
	
	} else{
	
		$consulta = true;
		
	}

}

/**
 * Função que monta o cabeçalho com os dados da obra selecionada
 *
 * @auhtor Bruno Ferreira
 * @return mixed
 * 
 */
function obras_cabecalho(){
	
	global $db;
	session_start();
	
	if(isset($_SESSION["obrid"])){
		
		$obrid = $_SESSION["obrid"];
		$obra  = obras_ver_obras($obrid);
		
		$percentualExecutado = intval(obras_percentual_executado($obrid));
		if(!$percentualExecutado) $percentualExecutado=0;
		
		$percentualExecutado.=" %";
		
		$titulos = array("Entidade ","Local da Obra ","Nome da Obra ","Órgão Responsável ","Valor do Contrato (R$)","(%) Concluído");
		$obra_list = array($obra["entidade"],$obra["unidade"],$obra['nome'],$obra['orgao'],number_format($obra['obrcustocontrato'],2,',','.'),$percentualExecutado);
		
		$cabecalho = "<table class=Tabela align=center>";
		
			for($i=0;$i<count($titulos);$i++){
			$cabecalho .= "<tr>";
				$cabecalho .= "<td width=100px class=SubTituloEsquerda style='text-align:right;' >";
					$cabecalho .= $titulos[$i];	
				$cabecalho .= "</td>";
				$cabecalho .= "<td width=80% class=SubTituloDireita style='text-align:left;background:#EEE;' >";
					$cabecalho .= $obra_list[$i];			
				$cabecalho .= "</td>";
			$cabecalho .= "</tr>";
			}
			
		$cabecalho .= "</table>";
			
		return $cabecalho; 
		
	}else{
		return "<br /><br /><hr /><center>Não existe nenhuma obra escolhida ...</center><br /><br /><hr />";
	}
}


// -------------------- CONTROLE DE OBRAS --------------------


/**
 * Função que cadastra as obras
 * 
 * @author Fernando Araújo Bagno da Silva
 * @param array $dados
 *
 */
function obras_cadastra_obras($dados){

	global $db;
	
	// Concatena os valores das coordenadas geográficas
	if(trim($dados["graulatitude"]) !== ''){
		$latitude  = "'" . $dados["graulatitude"] . "." .  $dados["minlatitude"] . "." . $dados["seglatitude"]. "." . $dados["pololatitude"] . "'";
		$longitude = "'" . $dados["graulongitude"] . "." . $dados["minlongitude"] . "." . $dados["seglongitude"] . "'";
	}else{
		$longitude = 'NULL';
		$latitude  = 'NULL';
	}
	
	// Trata os dados para serem inseridos no banco
	foreach($dados as $chave=>$valor){	
		if($valor == ""){
			$dados[$chave] = 'NULL';
		} else {
			$dados[$chave] = "'" . pg_escape_string(trim($valor))  .  "'";
		}
	}
	
	// Insere os dados na tabela endereco
	$sql = "
		INSERT INTO 
			entidade.endereco (endcep, endlog, endcom,
							   endbai, muncod, estuf,
							   endnum, medlatitude, medlongitude,
							   endstatus) 
		VALUES 
			 (" . obras_formata_cep($dados["endcep"]) . ", {$dados["endlog"]}, {$dados["endcom"]},
			 {$dados["endbai"]}, {$dados["muncod"]}, {$dados["estuf"]},
			 {$dados["endnum"]}, {$latitude}, {$longitude}, 'A') 
		RETURNING 
			endid";

	$endid = $db->pegaUm($sql);

	// Insere os dados na tabela obrainfraestrutura
	$sql = "";
	$sql = "
		INSERT INTO 
			obras.obrainfraestrutura (orgid, stoid, entidunidade, 
									  obrdescundimplantada,
									  obrdesc, 
									  endid, 
									  tobraid, 
									  obrdtinicio,
									  obrdttermino, 
									  obrpercexec, 
									  obrcustocontrato,
									  obrqtdconstruida, 
									  umdidobraconstruida, 
									  obrcustounitqtdconstruida, 
									  entidempresaconstrutora,
									  obsstatus,
									  obsobra, 
									  usucpf,
									  entidcampus)
		VALUES 
			({$dados["orgid"]}, 
			 {$dados["stoid"]}, 
			 {$dados["entid"]}, 
			 {$dados["obrdescundimplantada"]},
			 {$dados["obrdesc"]}, 
			 {$endid}, 
			 {$dados["tobraid"]}, 
			 {$dados["obrdtinicio"]},
			 {$dados["obrdttermino"]}, 
			 0.00,  
			 " . obras_formata_numero($dados["obrcustocontrato"]) . ",
			 " . obras_formata_numero($dados["obrqtdconstruida"]) . ",
			 " . obras_formata_numero($dados["umdidobraconstruida"]) . ", 
			 " . obras_formata_numero($dados["obrcustounitqtdconstruida"]) . ", 
			 {$dados["entidempresa"]}, 
			 'A', 
			 {$dados["obsobra"]}, 
			 '{$_SESSION["usucpf"]}',
			 {$dados["entidcampus"]}) 
		RETURNING 
			obrid";	
	
	$obrid = $db->pegaUm($sql);
	
	// Cria a sessão com o ID da obra
	$_SESSION["obrid"] = $obrid;
	
	// Insere os dados na tabela de responsáveis
	if (is_array($dados["tprcid"])){
		foreach ($dados as $chave=>$valor){
			$sql = "";
			$sql = "
				INSERT INTO
					obras.responsavelcontatos (entid, tprcid,
					 						   recostatus, recodtinclusao)
				VALUES 
					({$chave}, {$valor}, 'A', 'now()') 
				RETURNING 
					recoid";
			
			$recoid = $db->pegaUm($sql);
			
			// Cria o relacionamento entre o responsável e a obra
			$sql = "";
			$sql = "
				INSERT INTO obras.responsavelobra (recoid, obrid)
				VALUES ({$recoid}, {$_SESSION["obrid"]})";
			
			$db->executar($sql);
			
		}
	}
	
	$db->commit();
	
	// Pega o módulo atual
	$modulo_atual   = $_SERVER["REQUEST_URI"];
	$posicao_inicio = strpos($caminho_atual, 'modulo=');
	$posicao_fim    = strpos($caminho_atual, '&');
	$modulo_atual   = substr($modulo_atual, $posicao_inicio, $posicao_fim);
	
	$db->sucesso($modulo_atual, 'A');
	
}

/**
 * Função que atualiza a obra
 *
 * @author Fernando Araújo Bagno da Silva
 * @param array $dados
 * 
 */
function obras_atualiza_obras($dados){
	
	global $db;
	
	// Concatena os valores das coordenadas geográficas
	if(trim($dados["graulatitude"]) !== ''){
		$latitude  = $dados["graulatitude"] . "." .  $dados["minlatitude"] . "." . $dados["seglatitude"]. "." . $dados["pololatitude"];
		$longitude = $dados["graulongitude"] . "." . $dados["minlongitude"] . "." . $dados["seglongitude"];
	}else{
		$longitude = '';
		$latitude  = '';
	}
	
	// Atualiza os responsáveis
	if (is_array($dados["tprcid"])){
		
		$db->executar("
				UPDATE 
					obras.responsavelcontatos oc 
				SET
					recostatus = 'I'
				FROM
					obras.responsavelobra oo
				WHERE
					oc.recoid = oo.recoid AND
					oo.obrid = {$_SESSION["obrid"]}");
		
		$db->executar("DELETE FROM obras.responsavelobra WHERE obrid = {$_SESSION["obrid"]}");

		
		foreach ($dados["tprcid"] as $chave=>$valor){
			$sql = "";
			$sql = "
				INSERT INTO
					obras.responsavelcontatos (entid, tprcid,
					 						   recostatus, recodtinclusao)
				VALUES 
					({$chave}, {$valor}, 'A', 'now()') 
				RETURNING 
					recoid";
			
			$recoid = $db->pegaUm($sql);
			
			// Cria o relacionamento entre o responsável e a obra
			$sql = "";
			$sql = "
				INSERT INTO obras.responsavelobra (recoid, obrid)
				VALUES ({$recoid}, {$_SESSION["obrid"]})";
			
			$db->executar($sql);
		
		}
	}
	
	// Trata os dados para serem inseridos no banco
	foreach($dados as $chave=>$valor){
		if($valor == ""){
			$dados[$chave] = 'NULL';
		} else {
			$dados[$chave] = "'" . pg_escape_string(trim($valor))  .  "'";
		}
	}

	// Atualiza a tabela de endereco
	$db->executar("
				UPDATE 
					entidade.endereco en
				SET
					endcep       = " . obras_formata_cep($dados["endcep"]) . ",
					endlog       = {$dados["endlog"]},
					endcom       = {$dados["endcom"]},
					endbai       = {$dados["endbai"]},
					muncod       = {$dados["muncod"]},
					estuf        = {$dados["estuf"]},
					endnum       = {$dados["endnum"]},
					medlatitude  = '{$latitude}',
					medlongitude = '{$longitude}'
				FROM
					obras.obrainfraestrutura o
				WHERE
					o.endid = en.endid AND
					o.obrid = {$_SESSION["obrid"]}");
	
	//Atualiza a tabela de obras
	$db->executar("
			UPDATE 
				obras.obrainfraestrutura 
			SET 
				orgid = {$dados["orgid"]},
				entidunidade = {$dados["entid"]},
				obrdescundimplantada = {$dados["obrdescundimplantada"]},
				obrdesc = {$dados["obrdesc"]},
				tobraid = {$dados["tobraid"]},
				stoid = {$dados["stoid"]},
				obrdtinicio = {$dados["obrdtinicio"]},
				obrdttermino = {$dados["obrdttermino"]},
				obrpercbdi = " . obras_formata_numero($dados["obrpercbdi"]) . ",
				obrcustocontrato = " . obras_formata_numero($dados["obrcustocontrato"]) . ",
				obrqtdconstruida = " . obras_formata_numero($dados["obrqtdconstruida"]) . ",
				umdidobraconstruida = " . obras_formata_numero($dados["umdidobraconstruida"]) . ",
				obrcustounitqtdconstruida = " . obras_formata_numero($dados["obrcustounitqtdconstruida"]) . ",
				entidempresaconstrutora = {$dados["entidempresa"]},
				obsobra = {$dados["obsobra"]},
				usucpf = '{$_SESSION["usucpf"]}',
				entidcampus = {$dados["entidcampus"]}
			WHERE 
				obrid = {$_SESSION["obrid"]}");
	
	$db->commit();
	
	// Pega o módulo atual
	$modulo_atual   = $_SERVER["REQUEST_URI"];
	$posicao_inicio = strpos($caminho_atual, 'modulo=');
	$posicao_fim    = strpos($caminho_atual, '&');
	$modulo_atual   = substr($modulo_atual, $posicao_inicio, $posicao_fim);
	
	$db->sucesso($modulo_atual, 'A');
	
}

/**
 * Função que deleta uma obra
 *
 * @author Fernando Araújo Bagno da Silva
 * @param integer $obrid
 * 
 */
function obras_deleta_obras($obrid){
	
	global $db;
	$db->executar("
			UPDATE 
				obras.obrainfraestrutura 
			SET 
				obsstatus = 'I' WHERE obrid = " . $obrid);
	
	$db->commit();
	$_REQUEST["acao"] = "A";
	$db->sucesso("inicio");
	
}

/**
 * Função que busca os dados da obra
 *
 * @author Fernando Araújo Bagno da Silva
 * @param string $obrid
 * @return array
 * 
 */
function obras_busca_obras($obrid){
	
	global $db;
	
	$dados = $db->pegaLinha("
						SELECT 
							* 
						FROM 
							obras.obrainfraestrutura ob 
						WHERE 
							ob.obrid = {$obrid}");
	
	return $dados;
			
}


// -------------------- CONTROLE DE ETAPAS DA OBRAS --------------------


/**
 * Função que monta a lista com as etapas cadastradas na obra
 * 
 * @author Felipe de Oliveira Carvalho
 * @param string $dis
 * @return mixed
 * 
 */
function obras_monta_tabela_etapas($dis) {	
	$sql = pg_query("
		SELECT 
			i.itcid,
			i.icovlritem,
			i.icopercsobreobra,
			i.icopercexecutado,
			ic.itcdesc,
			ic.itcdescservico
		FROM 
			obras.itenscomposicaoobra i,
			obras.itenscomposicao ic 
		WHERE 
			i.obrid = ".$_SESSION["obrid"]." 
			and i.itcid = ic.itcid 
		ORDER BY 
			i.icoordem");
	
	$count = 1;
	$soma = 0;
	$somav = 0;
	
	$controleLinha = 1;
	
	while (($dados = pg_fetch_array($sql)) != false) {
		$itcid = $dados['itcid'];
		$icovlritem = $dados['icovlritem'];
		$itcdesc = $dados['itcdesc'];
		$icopercsobreobra = $dados['icopercsobreobra'];
		$icopercexecutado = $dados['icopercexecutado'];
		$itcdescservico = $dados['itcdescservico'];
		
		$somav = bcadd($somav, $icovlritem, 2);
		
		$icovlritem = number_format($icovlritem,2,',','.'); 
		
		$soma = bcadd($soma, $icopercsobreobra, 2);
		
		
		$cor = "#f4f4f4";
		
		$count++;
		
		$nome = "linha_".$itcid;
		
		if ($count % 2){
			$cor = "#e0e0e0";
		}
		
		if ($itcdescservico!='')
		$title = "onmouseover=\"return escape('$itcdescservico');\"";
		else
		$title = "";
		
		
		$sql_excluir = pg_query("
						SELECT 
							count(*) as num 
						FROM 
							obras.itenscomposicaoobra itco
						INNER JOIN
							obras.supervisaoitenscomposicao sup
						ON 
							itco.icoid = sup.icoid 
						WHERE 
							itco.obrid = ".$_SESSION["obrid"]." 
							AND itco.itcid = ".$itcid."
							AND (itco.icodtinicioitem is not null
							OR itco.icopercexecutado is not null
							OR itco.icodterminoitem is not null)");
		
		$dados_e = pg_fetch_array($sql_excluir);
		
		if($dis == "") {
			if($dados_e["num"] == 0) {
				$botaoExcluir = "<span onclick='excluiItem(this.parentNode.parentNode.rowIndex);'><img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
			} else {				
				$botaoExcluir = "<span onclick='alert(\"Existe cronograma e/ou supervisão cadastrado para esta etapa.\");'><img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
			}
		}
		else {
			$botaoExcluir = "<span><img src='/imagens/excluir_01.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
		}
		if($controleLinha == 1) {
			 $detalhesSetaCima = "<span><a onclick=\"troca_linhas('tabela_etapas','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cimad.gif' id='sobe_dis' border='0' title='Subir'></a></span>";
			 if(pg_num_rows($sql) == 1) {
			 	$detalhesSetaBaixo = "<span><a onclick=\"troca_linhas('tabela_etapas','baixo',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
			 }else {
			 	$detalhesSetaBaixo = "<span><a onclick=\"troca_linhas('tabela_etapas','baixo',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_baixo.gif' style='cursor:pointer;' border='0' title='Descer'></a></span>";
			 }
		}elseif(pg_num_rows($sql) == $controleLinha) {
			 $detalhesSetaCima = "<span><a onclick=\"troca_linhas('tabela_etapas','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cima.gif' style='cursor:pointer;' border='0' title='Subir'></a></span>";
			 $detalhesSetaBaixo = "<span><a onclick=\"troca_linhas('tabela_etapas','baixo',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
		}else {
			 $detalhesSetaCima = "<span><a onclick=\"troca_linhas('tabela_etapas','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cima.gif' style='cursor:pointer;' border='0' title='Subir'></a></span>";
			 $detalhesSetaBaixo = "<span><a onclick=\"troca_linhas('tabela_etapas','baixo',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_baixo.gif' style='cursor:pointer;' border='0' title='Descer'></a></span>";
		}
		

		echo "			
			<tr id=\"$nome\"  onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
				<td align=\"center\">
				$detalhesSetaCima
				$detalhesSetaBaixo
				</td>
				<td align=\"center\">
				$botaoExcluir
				</td>
				<td $title>
					$itcdesc	
				</td>
				<td align=\"center\">
					<input class='CampoEstilo' type='text' id='valoritem_$itcid' size='15' maxlength='14' value='" . $icovlritem . "' onkeypress='reais(this,event)' onkeydown='backspace(this,event);' onblur='verificaVist(this,\"" . $icovlritem . "\",\"" . $icopercexecutado . "\"); preencheRef(\"" . $itcid . "\",\"".$icovlritem."\"); calculaTotal();'  $dis >					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='referente_$itcid' size='6' maxlength='6' value='".str_replace('.',',',$icopercsobreobra)."' onkeypress='reais(this,event)' onkeydown='backspace(this,event);'  onblur='verificaVist(this,\"" . str_replace('.',',',$icopercsobreobra) . "\",\"" . $icopercexecutado . "\"); preencheVal(\"" . $itcid . "\",\"".str_replace('.',',',$icopercsobreobra)."\"); calculaTotal();'  $dis> %					
				</td>
			</tr>
		";
		$controleLinha++;

					
	}
	
	if($count != 1) {			
		echo "			
			<tr id=\"tr_total\" bgcolor=\"#FFFFFF\">
				<td></td>
				<td></td>
				<td align=\"right\"><strong>Total</strong></td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='totalv' size='15' maxlength='14' value='".number_format($somav,2,',','.')."' disabled=\"disabled\">					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='total' size='6' maxlength='6' value='".str_replace('.',',',$soma)."' disabled=\"disabled\"> %					
				</td>
			</tr>
			<tr id=\"tr_vlcontrato\" bgcolor=\"#FFFFFF\">
				<td></td>
				<td></td>
				<td align=\"right\"><strong>Valor do Contrato</strong></td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='vl_contrato' size='15' maxlength='14' value='".number_format($_SESSION["obrcustocontrato"],2,',','.')."' disabled=\"disabled\">					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='vl_porcento' size='6' maxlength='6' value='100,00' disabled=\"disabled\"> %					
				</td>
			</tr>
			<tr id=\"tr_vlrestante\" bgcolor=\"#FFFFFF\">
				<td></td>
				<td></td>
				<td align=\"right\"><strong>Valor Restante</strong></td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='rest_totalv' size='15' maxlength='14' value='".number_format($_SESSION["obrcustocontrato"]-$somav,2,',','.')."' disabled=\"disabled\">					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='rest_total' size='6' maxlength='6' value='".number_format(100-$soma,2,',','.')."' disabled=\"disabled\"> %					
				</td>
			</tr>
		";
	}
}

/**
 * Função que valida se um item pode ou não ser excluido
 *
 * @author Felipe de Oliveira Carvalho
 * @param integer $itcid
 * @return integer
 * 
 */
function obras_pode_excluir_etapas($itcid) {
	$sql_excluir = pg_query("SELECT count(*) as num 
						FROM obras.itenscomposicaoobra itco 
						WHERE itco.obrid = ".$_SESSION["obrid"]." AND itco.itcid = ".$itcid."
							  AND (itco.icovlritem is not null
							  OR itco.icodtinicioitem is not null
							  OR itco.icodterminoitem is not null
							  OR itco.icopercprojperiodo is not null)");
	$dados = pg_fetch_array($sql_excluir);
		
	return $dados["num"];
}

/**
 * Função que monta a lista com as etapas existentes no popup
 *
 * @author Fernando Araújo Bagno da Silva
 * @return mixed
 * 
 */
function obras_monta_popup_etapas(){
	$sql = pg_query("SELECT itcid, itcdesc, itcdescservico FROM obras.itenscomposicao order by itcordem");
	$count = "1";
	while (($dados = pg_fetch_array($sql)) != false){
		$itcid = $dados['itcid'];
		$itcdesc = $dados['itcdesc'];
		$itcdescservico = $dados['itcdescservico'];
		$cor = "#f4f4f4";
		$count++;
		$nome = "etapa_".$itcid;
		if ($count % 2){
			$cor = "#e0e0e0";
		}
		
		if (trim($itcdescservico)!='')
		$title = "onmouseover=\"return escape('$itcdescservico');\"";
		else
		$title = "";
		
		if(obras_pode_excluir_etapas($itcid)) 
			$key = "accesskey=\"x\"";
		else 
			$key = "";
		
		echo "
			<script type=\"text/javascript\"> id_etapas.push('$nome'); </script>
			<tr bgcolor=\"$cor\"  onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
				<td $title>
				<input id=\"".$nome."\" name=\"".$itcdesc."\" type=\"checkbox\" value=\"" . $itcid . "\" $key onclick=\"marcaItem('".$itcdesc."', ".$itcid.", '".$nome."', ".obras_pode_excluir_etapas($itcid).");\">" . $itcdesc . "
				</td>
			</tr>
		";	
	};
}


// -------------------- CONTROLE DE CRONOGRAMAS --------------------


/**
 * Função que cadastra o cronograma dos itens da obra
 *
 * @author Bruno Ferreira
 * @param array $dados
 * 
 */
function obras_cadastra_cronograma($dados){
	
	global $db;
	
	foreach($dados as $campo=>$valor){
		if($valor == "" ){
			$dados[$campo] = 'NULL';
		} else {
			$dados[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
		}
	}
	
	$i = 0;
	
	foreach($dados as $nome=>$valor){
		$pos = strpos($nome,"_");
		if($pos > 0){
			if(($i % 3) >= 0){
				$pos = strpos($nome,"_");
				$nome = substr($nome,0,$pos);
				if ($nome != "item"){
					$valor = str_replace(".","",$valor);
					$valor = str_replace(",",".",$valor);
					$query .= $nome." = ".$valor;
				}
				
			}
			if(($i % 3) > 1){
				$codigo = $valor;
			}elseif(($i % 3) < 1){
				$query .= ", ";
			}
			if(($i % 3) == 1){
				$query .= " WHERE itcid =" . $codigo . " AND obrid = '{$_SESSION["obrid"]}'";
				$sql = "UPDATE obras.itenscomposicaoobra SET " . $query;
				$query = "";
				
				$db->executar($sql);
				$db->commit();
			}
		} 
		$i++;
	}
	
	$_REQUEST["acao"] = "A";
	$db->sucesso("principal/cronograma");
}


// -------------------- CONTROLE DE CONTRATAÇÃO DA OBRA --------------------

/**
 * Função que busca os dados da contratação da obra
 *
 * @author Fernando A. Bagno da Silva
 * @param integer $obrid
 * @return array
 * 
 */
function obras_busca_contratacao($obrid){
	
	global $db;
	
	$dados = $db->pegaLinha("
						SELECT 
							* 
						FROM 
							obras.formarepasserecursos fr
						INNER JOIN
							obras.obrainfraestrutura ob ON ob.obrid = fr.obrid
						WHERE 
							fr.obrid = {$obrid} AND frrstatus = 'A'");
	
	return $dados;
}

/**
 * Função que cadastra a contratação da obra
 *
 * @author Robison
 * @param array $obra
 * 
 */
function obras_cadastrar_contratacao_obras($obra){
	
	global $db;
	
	$insert_dados = array();
	$flcid_tela = array();
	$flcid_banco = array();   
	$sql_insert=array();

	if(is_array($obra['tflid'])){
		foreach($obra['tflid'] as $key=>$item){
			$flcpubleditaldtprev = $obra['flcpubleditaldtprev'][$key] ? "'".$obra['flcpubleditaldtprev'][$key]."'":'null';
			$flcdtrecintermotivo = $obra['flcdtrecintermotivo'][$key] ? "'".$obra['flcdtrecintermotivo'][$key]."'":'null';
			$flcrecintermotivo = $obra['flcrecintermotivo'][$key] ? "'".$obra['flcrecintermotivo'][$key]."'":'null';
			$flcordservdt = $obra['flcordservdt'][$key] ? "'".$obra['flcordservdt'][$key]."'":'null';
			$flcordservnum = $obra['flcordservnum'][$key] ? "'".$obra['flcordservnum'][$key]."'":'null';
			$flchomlicdtprev = $obra['flchomlicdtprev'][$key] ? "'".$obra['flchomlicdtprev'][$key]."'":'null';
			$flcaberpropdtprev = $obra['flcaberpropdtprev'][$key] ? "'".$obra['flcaberpropdtprev'][$key]."'":'null';
			$_sql ="";
                                 
			if(!is_numeric($obra['flcid'][$key])){
				$_sql .= "INSERT INTO obras.faselicitacao (tflid,obrid,flcstatus,";
				$_sql .= "flcpubleditaldtprev,flcdtrecintermotivo,flcrecintermotivo,";
				$_sql .= "flcordservdt,flcordservnum,flchomlicdtprev,flcaberpropdtprev) ";
				$_sql .= "VALUES ";
				$_sql .= "(".$item.",".$obra['obrid'].",'A',";
				$_sql .= "".$flcpubleditaldtprev.",".$flcdtrecintermotivo.",".$flcrecintermotivo.",";
				$_sql .= "".$flcordservdt.",".$flcordservnum.",".$flchomlicdtprev.",".$flcaberpropdtprev."";
				$_sql .= ")";
				$db->executar($_sql);
			}
		}
	}

	if(is_array($obra['acaoFases'])){
		foreach($obra['acaoFases'] as $key=>$item){
			if(is_numeric($item)){
				$_sql ="";
				$_sql .= "UPDATE obras.faselicitacao SET flcstatus ='I' WHERE flcid = ".$item;
				$db->executar($_sql);
			}
		}
	}

	// Insere os dados da tabela formarepasserecursos
	
	$sql = "UPDATE obras.formarepasserecursos SET frrstatus = 'I' where obrid = '{$_SESSION["obrid"]}'";
	$db->executar($sql);
	
	$arrayformarepasserecursos=array('frpid','obrid','covcod','frrconventbenef','frrconvnum','frrconvobjeto','frrconvvlr','frrconvvlrconcedente','frrconvvlrconcenente','frrdescinstituicao','frrdescnumport','frrdescobjeto','frrdescvlr','frrdescdtviginicio','frrdescdtvigfinal','frrobsrecproprio');
	$campos = "";
	$valores = "";
	$camposSet = "";

	$obra['frrdescvlr'] = obras_formata_numero($obra['frrdescvlr']);
	$obra['frrconvvlr'] = obras_formata_numero($obra['frrconvvlr']);
	$obra['frrconvvlrconcedente'] = obras_formata_numero($obra['frrconvvlrconcedente']);
	$obra['frrconvvlrconcenente'] = obras_formata_numero($obra['frrconvvlrconcenente']);

	if($obra["frpid"] == "2"){ //Convênio
		$obra['frrdescinstituicao'] = "";
		$obra['frrdescnumport'] = "";
		$obra['frrdescobjeto'] = "";
		$obra['frrdescvlr'] = "";
		$obra['frrobsrecproprio'] = "";
	}

	if($obra["frpid"] == "3"){ //Descentralização
		$obra['frrconventbenef'] = "";
		$obra['frrconvnum'] = "";
		$obra['frrconvobjeto'] = "";
		$obra['frrconvvlr'] = "";
		$obra['frrconvvlrconcedente'] = "";
		$obra['frrconvvlrconcenente'] = "";
		$obra['total'] = "";
		$obra['frrdescdtviginicio'] = "";
		$obra['frrdescdtvigfinal'] = "";
		$obra['frrobsrecproprio'] = "";
	}

	if($obra["frpid"] == "4"){//Recurso Próprio
		$obra['frrconventbenef'] = "";
		$obra['frrconvnum'] = "";
		$obra['frrconvobjeto'] = "";
		$obra['frrconvvlr'] = "";
		$obra['frrconvvlrconcedente'] = "";
		$obra['frrconvvlrconcenente'] = "";
		$obra['total'] = "";
		$obra['frrdescdtviginicio'] = "";
		$obra['frrdescdtvigfinal'] = "";
		$obra['frrdescinstituicao'] = "";
		$obra['frrdescnumport'] = "";
		$obra['frrdescobjeto'] = "";
		$obra['frrdescvlr'] = "";
	}

	foreach($arrayformarepasserecursos as $key){
		//if($obra[$key]){
		if($obra[$key] == ""){
			$camposSet .= " ".$key." = NULL, ";
			$campos  .="".$key.",";
			$valores .=" NULL,";
		}else{
			$camposSet .= " ".$key." = '".$obra[$key]."', ";
			$campos  .="".$key.",";
			$valores .="'".$obra[$key]."',";
		}
		//}
	}

	$campos  .="frrstatus, frrdtinclusao";
	$valores .="'A', now()";
	$camposSet .= " frrstatus = 'A', frrdtinclusao = now() ";
	if($obra['frrid']){
		$sql = "UPDATE obras.formarepasserecursos SET ".$camposSet." WHERE frrid = ".$obra['frrid']."";         
	}else{
		$sql = "INSERT INTO obras.formarepasserecursos (".$campos.") VALUES (".$valores.")";  
	}

    $db->executar($sql);
    $db->commit();
    $_REQUEST["acao"] = "A";
	$db->sucesso("principal/contratacao_da_obra");
}


// -------------------- CONTROLE DE AQUISIÇÃO DE EQUIPAMENTOS --------------------

/**
 * Função que busca os dados da aquisição
 *
 * @author Alex Pereira
 * @param integer $id
 * @return array
 */
function obras_busca_aquisicao($id){
		
		global $db;
		
		$result = $db->executar("
							SELECT 
								* 
							FROM 
								obras.aquisicaoequipamentos
							WHERE
								obrid = ".$id);
		
		$db->commit();
		return pg_fetch_assoc($result);				
	
	}

/**
 * Função que cadastra a aquisição de equipamentos
 *
 * @author Alex Pereira
 * @param array $obra
 * 
 */
function obras_cadastrar_aquisicao($obra){
	
	global $db;
	
	$aeqid = $db->pegaUm("
					SELECT 
						aeqid 
					FROM 
						obras.aquisicaoequipamentos 
					WHERE 
						obrid = '{$_SESSION["obrid"]}'");
	
	foreach($obra as $campo=>$valor){
		if (!is_array($valor)){
			if($valor == "" ){
				$obra[$campo] = 'NULL';
			} else {
				$obra[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
			}
		}
	}
	
	foreach($obra as $nome=>$valor){		
		if(is_array($valor)){
			if(preg_match("/^tmaid/",$nome)){
				$tmaid = $valor;
			}
		}
	}
	
	// Insere os dados na tabela aquisicaoequipamentos 
	// se não houver aquisicaoequipamentos cadastrada para a obra selecionada
	if (!$aeqid){
		$sql = "
			INSERT INTO 
				obras.aquisicaoequipamentos 
					(faeid,
					 obrid,
				 	 aeqdtpubledital,
				 	 aeqdtpublreslicitacao,
				 	 aeqobs,
				 	 aeqdtinclusao) 
			VALUES
				({$obra["faeid"]},
				 {$obra["obrid"]},
				 {$obra["aeqdtpubledital"]},
				 {$obra["aeqdtpublreslicitacao"]},
				 {$obra["aeqobs"]}, now()) returning aeqid ";
		
		$retorno = $db->pegaUm($sql);
		
		// Atualiza a tabela aquisicaoequipamentos setando o ID 
		// da última aquisicaoequipamentos cadastra
		$sql ="	UPDATE obras.aquisicaoequipamentos SET	aeqid = '{$retorno}' WHERE obrid = ".$_SESSION["obrid"];
		
		$db->executar($sql);
			
	}
		
	if ($aeqid){
		
		$sql = "
			UPDATE
				obras.aquisicaoequipamentos 
			SET
				faeid = {$obra["faeid"]},
				aeqdtpubledital = {$obra["aeqdtpubledital"]},
				aeqdtpublreslicitacao = {$obra["aeqdtpublreslicitacao"]},
				aeqobs = {$obra["aeqobs"]},
				aeqdtinclusao = now()
			WHERE
				aeqid = {$aeqid}";
		
				
		$db->executar($sql);
		
	}
	
	$db->commit();
	$_REQUEST["acao"] = "A";
	$db->sucesso("principal/aquisicao_equipamentos");	

}


// -------------------- CONTROLE DE PROJETOS --------------------


function obras_busca_projetos($id){
	
	global $db;
		
	$result = $db->executar("
						SELECT 
							* 
						FROM 
							obras.faseprojeto 
						INNER JOIN
							obras.formaelaboracao ON 
						WHERE obrid = " . $id);
	
	$db->commit();
	return pg_fetch_assoc($result);		
}

/**
 * Função que cadastra os projetos da obra
 *
 * @author Fernando A. Bagno da Silva
 * @param array $dados
 */
function obras_cadastrar_projeto($dados){
	
	global $db;
	
	//limpa campos
	if(!$dados['tpaid'] && !$dados['felid'] && !$dados['tfpid']){
		if($dados['fprid']){

			$SQL = "DELETE FROM obras.faselicitacaoprojetos WHERE fprid = ".$dados['fprid'];
			$db->executar($SQL);
		
			$SQL = "DELETE FROM obras.faseprojeto WHERE fprid = ".$dados['fprid'];
			$db->executar($SQL);
		
		}
		
		$db->commit();
		$_REQUEST["acao"] = "A";
		$db->sucesso("principal/projeto_arquitetonico");
		exit();
	}
	
	//insere e altera dados
	$campos = array();
	$_where = "";
	
	foreach($dados as $campo=>$valor){
		
		$search  = preg_match("/^ftp|^fpr|^fel|^tpa|^tfp/",$campo);
		if($search){
			
			if($valor){
				$tem_ponto = preg_match("/,/",$valor);
				if($tem_ponto){
					$valor = str_replace(".","",$valor);
					$valor = str_replace(",",".",$valor);
					
				}
				array_push($campos,array($campo=>$valor));
			}
		}
	}
	
	$total = count($campos);
	if($dados['fprid']){
		
		$sql = "UPDATE obras.faseprojeto SET ";
		$j=0;
		foreach($campos as $campo=>$valor){
			
			foreach($valor as $c=>$v)
			$sql .= $c."="."'".$v."'";
							
			if($j >= 0 && $j < ($total-1) )
				$sql .= ",";
				
			$j++;
			
		}	
		$sql .= " WHERE obrid=".$_SESSION['obrid']." AND fprid=".$dados['fprid'];	
		$query = $sql;
		
	}else{
		
		$sql = "INSERT INTO obras.faseprojeto (";
		$campo = "";
		$valor = "";
		
		for($k = 0;$k < $total ;$k++){
		
		$campo .= key($campos[$k]);	 
		$valor .= "'".current($campos[$k])."'";
		
			if($k >= 0 && $k < ($total-1) ){
				$campo .= ",";
				$valor .= ",";
			}	
			
		}
		$query = $sql.$campo.",obrid,fprstatus,fprdtinclusao) values (".$valor.",{$_SESSION['obrid']},'A','".Date('d/m/Y H:i:s')."');";
		
	}
	$db->executar($query);
	
	//insere dados na fase de licitação de projeto
	//pega o codigo da faseprojeto
	$fprid = ($db->pegaUm("SELECT fprid FROM obras.faseprojeto	WHERE obrid = {$_SESSION["obrid"]}"));
	
	$SQL = "DELETE FROM obras.faselicitacaoprojetos WHERE fprid = ".$fprid;
	$db->executar($SQL);
	
	if(is_array($dados['tflid'])){
		foreach($dados['tflid'] as $key=>$item){
		
			$tflid = $dados['tflid'][$key];
			$flcpubleditaldtprev = $dados['flcpubleditaldtprev'][$key];
			$flcdtrecintermotivo = $dados['flcdtrecintermotivo'][$key];
			$flcrecintermotivo = $dados['flcrecintermotivo'][$key];
			$flcordservdt = $dados['flcordservdt'][$key];
			$flcordservnum = $dados['flcordservnum'][$key];
			$flchomlicdtprev = $dados['flchomlicdtprev'][$key];
			$flcaberpropdtprev = $dados['flcaberpropdtprev'][$key];
			$_sql ="";
			
			if($tflid ==2){
				$flcdata = $flcpubleditaldtprev;
				$flcrecintermotivo = "";
				$flcordservnum = "";
				$flcdtrecintermotivo = "";
				$flcordservdt = "";
				$flchomlicdtprev = "";
				$flcaberpropdtprev = "";
			}
			if($tflid ==5){
				$flcdata = $flcdtrecintermotivo;
				$flcpubleditaldtprev = "";
				$flcordservnum = "";
				$flcordservdt = "";
				$flchomlicdtprev = "";
				$flcaberpropdtprev = "";
			}
			if($tflid ==6){
				$flcdata = $flcordservdt;
				$flcrecintermotivo = "";
				$flcdtrecintermotivo = "";
				$flchomlicdtprev = "";
				$flcaberpropdtprev = "";
				$flcdtrecintermotivo = "";
			}
			if($tflid ==9){
				$flcdata = $flchomlicdtprev;
				$flcrecintermotivo = "";
				$flcordservnum = "";
				$flcdtrecintermotivo = "";
				$flcordservdt = "";
				$flcpubleditaldtprev = "";
				$flcaberpropdtprev = "";
			}
			if($tflid ==7){
				$flcdata = $flcaberpropdtprev;
				$flcrecintermotivo = "";
				$flcordservnum = "";
				$flcdtrecintermotivo = "";
				$flcordservdt = "";
				$flcpubleditaldtprev = "";
				$flchomlicdtprev = "";
			}
							
			$_sql .= "INSERT INTO obras.faselicitacaoprojetos(tflid,fprid,tfpstatus,tfpdtfase,tfpnumos,tfpobsmotivo) ";
			$_sql .= "VALUES";
			$_sql .= "(".$item.",".$fprid.",'A',";
			$_sql .= "'".$flcdata."',";
			if($flcordservnum!="") $_sql .= "'".$flcordservnum."',";
			else $_sql .= "null,";
			if($flcrecintermotivo!="") $_sql .= "'".$flcrecintermotivo."'";
			else $_sql .= "null";
			$_sql .= ")";
			
			$db->executar($_sql);
		}
	}
	
	$db->commit();
	$_REQUEST["acao"] = "A";
	$db->sucesso("principal/projeto_arquitetonico");
	
}


// -------------------- CONTROLE DE RESPONSABILIDADES --------------------


/**
 * Função que pega os códigos do usuário logado no sistema para
 * verificar se o mesmo pode ou não cadastrar uma obra
 * 
 * @author Fernando Araújo Bagno da Silva
 * @return array
 * 
 */
function obras_podeCadastrarObra(){

	global $db;
	
	$sql = "
		SELECT 
			orgcod, ungcod 
		FROM 
			seguranca.usuario 
		WHERE 
			usucpf = '{$_SESSION["usucpf"]}'";
	
	$dados = $db->carregar($sql);
	
	return $dados;

}

/**
 * Função que verifica se o usuário possui perfil para acessar as páginas
 *
 * @author Fernando Araújo Bagno da Silva
 * @param array $pflcods
 * @return unknown
 * 
 */
function possuiPerfil( $pflcods )
{
	global $db;
	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
}

/**
 * Verifica se o perfil do usuário possui algum vínculo de responsabilidade
 *
 * @author Fernando Araújo Bagno da Silva
 * @return unknown
 * 
 */
function obras_possuiPerfilSemVinculo(){
	
	global $db;
	
	$sql = "
		SELECT
			count(*)
		FROM 
			seguranca.perfil p
		INNER JOIN 
			seguranca.perfilusuario u on
			u.pflcod = p.pflcod
		LEFT JOIN 
			obras.tprperfil tp on
			tp.pflcod = p.pflcod
		LEFT JOIN 
			obras.tiporesponsabilidade tr on
			tr.tprcod = tp.tprcod
		WHERE
			p.pflstatus = 'A' AND
			p.sisid = '15' AND
			u.usucpf = '" . $_SESSION['usucpf'] . "' AND
			tr.tprcod is null
	";
	return $db->pegaUm( $sql ) > 0;
}

/**
 * Pega o órgão que o usuário possui responsabilidade
 *
 * @author Fernando Araújo Bagno da Silva
 * @return mixed
 * 
 */
function obras_pegarOrgaoPermitido(){
	
	global $db;
	static $orgao = null;
	
	if ($orgao === null){
		if ($db->testa_superuser() || obras_possuiPerfilSemVinculo()){
			
			// pega todos os orgãos
			$sql = "
				SELECT
					o.orgdesc                                               as descricao,
	                o.orgid                                                 as id,
                	'/obras/obras.php?modulo=inicio&acao=A&org=' || o.orgid as link
				FROM
					obras.orgao o";
		}else {
			
			// pega o orgão do perfil do usuário
			$sql= "
				SELECT DISTINCT
					o.orgdesc                                               as descricao,
	                o.orgid                                                 as id,
                	'/obras/obras.php?modulo=inicio&acao=A&org=' || o.orgid as link
				FROM
					obras.orgao o
				INNER JOIN 
					obras.usuarioresponsabilidade ur ON
					o.orgcodigo = ur.orgcod
				INNER JOIN 
					seguranca.perfil p ON
					p.pflcod = ur.pflcod
				INNER JOIN 
					seguranca.perfilusuario pu ON
					pu.pflcod = ur.pflcod AND
					pu.usucpf = ur.usucpf
				WHERE
					ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
					ur.rpustatus = 'A' AND
					p.sisid =  '15'";
		}
		
		//dump($sql);
		$orgao = $db->carregar($sql);
		
		//die();
	}
	return $orgao;
}

/**
 * Pega as unidades que o usuário possui responsabilidade
 *
 * @author Fernando Araújo Bagno da Silva
 * @return mixed
 * 
 */
function obras_pegarUnidadesPermitidas(){
	
	global $db;
	static $unidades = null;
	
	if ($unidades === null){
		if ($db->testa_superuser() || obras_possuiPerfilSemVinculo()){
			
			// pega todas as unidades
			$sql = "
				SELECT
					unidsc
				FROM
					unidade
				WHERE
					orgcod = '". CODIGO_ORGAO_SISTEMA. "'";
		}else{
			
			// pega as unidades do perfil do usuário
			$sql = "
				SELECT
					u.unicod
				FROM
					u.unidade
				INNER JOIN 
					obras.usuarioresponsabilidade ur ON
					u.unicod = ur.unicod 
				INNER JOIN 
					seguranca.perfil p ON
					p.pflcod = ur.pflcod
				INNER JOIN 
					seguranca.perfilusuario pu ON
					pu.pflcod = ur.pflcod AND
					pu.usucpf = ur.usucpf
				WHERE
					ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
					ur.rpustatus = 'A' AND
					p.sisid =  '15'";
		}
	
		$dados = $db->carregar($sql);
		$dados = $dados ? $dados : array();
		$unidades = array();
		
		foreach ( $dados as $linha ){
			array_push( $unidades, $linha['unicod'] );
		}
	}
	return $unidades;	
}

/**
 * Pega as uf's que o usuário possui responsabilidades
 *
 * @author Fernando Araújo Bagno da Silva
 * @return array
 */
function obras_pegarUfsPermitidas(){
	
	global $db;
	static $ufs = null;
	
	if ($ufs === null){
		if ($db->testa_superuser() || obras_possuiPerfilSemVinculo()){
			
			// pega todos os estados
			$sql = "
				SELECT
					estuf
				FROM 
					territorios.estado";
		}else{
			
			// pega estados do perfil do usuário
			$sql = "
				SELECT
					e.estuf
				FROM 
					territorios.estado e
				INNER JOIN 
					obras.usuarioresponsabilidade ur on
					ur.estuf = e.estuf
				INNER JOIN 
					seguranca.perfil p on
					p.pflcod = ur.pflcod
				INNER JOIN 
					seguranca.perfilusuario pu on
					pu.pflcod = ur.pflcod and
					pu.usucpf = ur.usucpf
				WHERE
					ur.usucpf = '" . $_SESSION['usucpf'] . "' and
					ur.rpustatus = 'A' and
					p.sisid =  '15'
				GROUP BY
					e.estuf";
			
		}
		
		$dados = $db->carregar($sql);
		$dados = $dados ? $dados : array();
		$ufs = array();
		
		foreach ( $dados as $linha ){
			array_push( $ufs, $linha['estuf'] );
		}
	}
	return $ufs;
}


/**
 * Pega os municípios que o usuário possui responsabilidade
 *
 * @author Fernando Araújo Bagno da Silva
 * @return unknown
 */
function obras_pegarMunicipiosPermitidos(){
	
	global $db;
	static $municipios = null;
	
	if ($municipios === null){
		if ($db->testa_superuser() || obras_possuiPerfilSemVinculo()){
			
			// pega todos os estados
			$sql = "
				SELECT
					muncod
				FROM 
					territorios.municipio limit 200";
		
		}else if(obras_pegarUfsPermitidas()){
			
			// pega estados do perfil do usuário
			$sql = "
				SELECT
					muncod
				FROM 
					territorios.municipio
				WHERE 
					estuf in ('". implode( "','", obras_pegarUfsPermitidas() ) ."')";	
		
		}else{
			
			// pega estados do perfil do usuário
			$sql = "
				SELECT
					m.muncod
				FROM 
					territorios.municipio m
				INNER JOIN 
					obras.usuarioresponsabilidade ur on
					ur.muncod = m.muncod
				WHERE
					ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
					rpustatus = 'A'
				GROUP BY
					m.muncod";
			
		}
		
		$dados = $db->carregar( $sql );
		$dados = $dados ? $dados : array();
		$municipios = array();
		
		foreach ( $dados as $linha ){
			array_push( $municipios, $linha['muncod'] );
		}
	}
	return $municipios;
}


?>