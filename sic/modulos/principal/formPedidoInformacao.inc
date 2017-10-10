<?php 
function verificaFeriado($data)
{
	global $db;
	
	$sql  = "select 
				feddescricao 
			 from 
			 	public.feriados 
			 where 
			 	feddata = '{$data}'";
	
	return $db->pegaUm($sql);
}

function proximoDiaUtil($data)
{
	$proximo = somar_dias_uteis($data, 1);
	 
	if(verificaFeriado($proximo)){
		return proximoDiaUtil($proximo);
	}else{		
		return $proximo;
	}
}

if($_REQUEST['requisicao'] == 'novoRegistro'){
	unset($_SESSION['sic']['slcid']);
	die;
}

if($_REQUEST['requisicao'] == 'carregarSecretarias'){
	
	$sql = "
		Select 	s.secid as codigo, 
				s.secdsc as descricao
		From sic.secretaria s
		Inner Join public.unidade u using(unicod)
		Inner Join public.orgao o using(orgcod)
		Where u.unicod= '".$_REQUEST['unicod']."'
		group by 1,2
		order by 2
	";
	
	$db->monta_combo('secid', $sql, 'S', 'Selecione...', 'carregarSecretarias', '', '', '', 'S');
	die;
}

if($_REQUEST['requisicao'] == 'verificaDiaUtil'){
	
	$data 		= explode('-',$_REQUEST['data']);	
	$feriado 	= verificaFeriado($_REQUEST['data']);	
	$diasemana 	= date( "w", mktime(0,0,0,$data[1],$data[2],$data[0]) );

	if( in_array($diasemana, array(0,6)) || $feriado){
		echo proximoDiaUtil($_REQUEST['data']);
		die;
	}	
	die('ok');
}

if($_REQUEST['slcid']){
	$_SESSION['sic']['slcid'] = $_REQUEST['slcid'];
}
if($_SESSION['sic']['slcid']){
	unset($_SESSION['abamenu'][$abacod_tela]);
}

if($_REQUEST['requisicao'] == 'carregaProgramas'){
	
	$sql = "select 
				pr.prgid as codigo,
				pr.prgdsc as descricao 
			from 
				sic.programa pr
			inner join 
				sic.programasecretaria ps on ps.prgid = pr.prgid
			where 
				ps.secid = {$_REQUEST['secid']}
			order by 
				pr.prgdsc ";
	
	$db->monta_combo('prgid', $sql, 'S', 'Selecione...', '', '', '', '', 'N');
	die;
}

if($_REQUEST['requisicao'] == 'carregarMunicipios'){
	
	$sql = "select 
				muncod as codigo,
				mundescricao as descricao
			from 
				territorios.municipio
			where
				estuf = '{$_REQUEST['estuf']}'
			order by
				mundescricao";
	
	$db->monta_combo('muncod', $sql, 'S', 'Selecione...', '', '', '', '', 'N');
	die;
}

if($_REQUEST['requisicao'] == 'verificarCep'){
	
	$sql = "SELECT 
				muncod,
				logradouro,
				bairro,
				estado
			FROM 
				cep.v_endereco2
			WHERE 
				cep='".$_REQUEST['endcep']."' 
			ORDER BY 
				cidade ASC";
	
	$rs = $db->pegaLinha($sql);
	echo simec_json_encode($rs);
	die;
}

if($_REQUEST['requisicao'] == 'verificaEntidade'){
	
	if($_REQUEST['entnumcpfcnpj'] || $_REQUEST['entnumrg']){		
		
		if($_REQUEST['entnumcpfcnpj']){
			
			if($_REQUEST['tipopessoa'] == 'F'){
				$stWhere = ' and length(ent.entnumcpfcnpj) = 11 ';
			}else{
				$stWhere = ' and length(ent.entnumcpfcnpj) = 14 ';
			}
	
			$stWhereDoc = "ent.entnumcpfcnpj = '{$_REQUEST['entnumcpfcnpj']}'";
			
		}else{
			
			if($_REQUEST['tipo'] != 'entnumrg'){
				$stWhere = " and ent.entorgaoexpedidor is null ";
			}
			
			$stWhereDoc = "ent.entnumrg = '{$_REQUEST['entnumrg']}'";
		}
		
		$sql = "select 
					ent.entid, 
					ent.entnome,
					ent.entemail,
					ent.entorgaoexpedidor,
					ede.muncod,
					ede.estuf,
					ede.endcep,
					ede.endlog,
					ede.endcom,
					ede.endbai,
					ede.endnum
				from 
					entidade.entidade ent
				left join 
					entidade.endereco ede on ede.entid = ent.entid
				where 
					{$stWhereDoc}
				{$stWhere}";
			
		$rs = $db->pegaLinha($sql);
		echo simec_json_encode($rs);
		
	}else{
		
		simec_json_encode(array('retorno'=>'false'));
	}
	die;
}

if($_REQUEST['requisicao'] == 'salvaDados'){
	if($_SESSION['sic']['slcid']){
		$slcidteste = "AND slcid != {$_SESSION['sic']['slcid']}";
	}
	$sqlChecaRepetido = "SELECT
							min(slcid)
						FROM
							sic.solicitacao
						WHERE
							slcpergunta = '{$_REQUEST['slcpergunta']}'
						{$slcidteste}";
// 	ver($sqlChecaRepetido,d);
	$ChecaRepetido= $db->pegaUm($sqlChecaRepetido);
	
	if($ChecaRepetido){
		$sqlNumSicRep = "SELECT
							'Número SIC:'|| slcnumsic as numerosic
						FROM
							sic.solicitacao
						WHERE
							slcid = {$ChecaRepetido}";
		$NumSicRep= $db->pegaUm($sqlNumSicRep);
		
		$_POST['slcdemandarepetida'] = 't';
		$_POST['slcrefsolicrepetida'] = $NumSicRep;
	}
	$_REQUEST['endcep'] 			= str_replace(array('.','-'), '', $_REQUEST['endcep']);
	$_REQUEST['entnumcpfcnpj'] 		= str_replace(array('.','-','/'), '', $_REQUEST['entnumcpfcnpj']);
	$_REQUEST['slcdtinclusao']		= formata_data_sql($_REQUEST['slcdtinclusao']);
	$_REQUEST['tpdid']				= $_REQUEST['tpdid'] ? $_REQUEST['tpdid'] : 'null';
	$_REQUEST['entnumcomercial']	= str_replace(array('.','-'), '', $_REQUEST['entnumcomercial']);
	$_REQUEST['entnumfax']			= str_replace(array('.','-'), '', $_REQUEST['entnumfax']);
	$_REQUEST['slcpergunta'] 		= addslashes( $_REQUEST['slcpergunta']);
	$_REQUEST['endcep']				= trim($_REQUEST['endcep']);

	if($_REQUEST['slcid']){
					
		$entid = atualizaDadosEntidade($_REQUEST);
		
		$sql = "update sic.solicitacao set
					entid 				  = ".($entid ? $entid : 'null').",
					tprid 				  = {$_REQUEST['tprid']}, 
					tpdid				  = {$_REQUEST['tpdid']},
					tpsid				  = {$_REQUEST['tpsid']},
					slcnumsic 			  = '{$_REQUEST['slcnumsic']}', 
					slctipopessoa 		  = '{$_REQUEST['slctipopessoa']}', 
					slcnomerepresentante  = '{$_REQUEST['slcnomerepresentante']}', 
					slccargorepresentante = '{$_REQUEST['slccargorepresentante']}', 
					slcpergunta			  = '{$_REQUEST['slcpergunta']}',
					slcdtalteracao		  = now(),					 
					usucpfalteracao 	  = '{$_SESSION['usucpf']}',
					slcdtinclusao		  = '{$_REQUEST['slcdtinclusao']}',
					secid				  = {$_REQUEST['secid']},
					unicod				  = '{$_REQUEST['unicod']}',
					slcprioridade         = '{$_REQUEST['slcprioridade']}',
					prgid				  = ".($_POST['prgid'] ? $_POST['prgid'] : 'null').",
					proid				  = ".($_POST['proid'] ? $_POST['proid'] : 'null').",
					escid				  = ".($_POST['escid'] ? $_POST['escid'] : 'null').",
					slcclassificacao	  = ".($_POST['slcclassificacao'] ? "'".$_POST['slcclassificacao']."'" : 'null').",
					slcdemandarepetida	  = ".($_POST['slcdemandarepetida'] ? "'".$_POST['slcdemandarepetida']."'" : 'null').",
					slcrefsolicrepetida	  = ".($_POST['slcrefsolicrepetida'] ? "'".$_POST['slcrefsolicrepetida']."'" : 'null').",
					--slccritico			  = '{$_POST['slcclassificacao']}',
                                        slcpalavraschave = ".($_REQUEST['slcpalavraschave'] ? "'".$_REQUEST['slcpalavraschave']."'" : 'null').",
                                        classificacaoconteudo = ".($_REQUEST['classificacaoconteudo'] ? "'".$_REQUEST['classificacaoconteudo']."'" : 'null').",
                                        slctelefone = ".($_REQUEST['slctelefone'] ? "'".$_REQUEST['slctelefone']."'" : 'null')."
				where 
					slcid = {$_REQUEST['slcid']}";
		$db->executar($sql);
		
		$slcid = $_REQUEST['slcid'];
		
		
	}else{
			
		$entid = atualizaDadosEntidade($_REQUEST);
			
		$sql = "insert into sic.solicitacao
					(
						entid, 
						tprid, 
						slcnumsic, 
						slctipopessoa, 
						slcnomerepresentante, 
						slccargorepresentante, 
						slcpergunta, 
						usucpfinclusao, 
						tpdid, 
						slcdtinclusao, 
						tpsid, 
						secid,
						unicod,
                        slcprioridade,
                        prgid,
                        proid,
                        escid,
                        slcclassificacao,
                        slcdemandarepetida,
                        slcrefsolicrepetida,
						--slccritico, 
                                                slcpalavraschave,
                                                classificacaoconteudo, 
                                                slctelefone
					)
				values
					(
						".($entid ? $entid : 'null').", 
						{$_REQUEST['tprid']}, 
						'{$_REQUEST['slcnumsic']}', 
						'{$_REQUEST['slctipopessoa']}',
						'{$_REQUEST['slcnomerepresentante']}', 
						'{$_REQUEST['slccargorepresentante']}', 
						'{$_REQUEST['slcpergunta']}',
						'{$_SESSION['usucpf']}', 
						{$_REQUEST['tpdid']}, 
						'{$_REQUEST['slcdtinclusao']}', 
						{$_REQUEST['tpsid']}, 
						{$_REQUEST['secid']},
						'{$_REQUEST['unicod']}',
						'{$_REQUEST['slcprioridade']}',
						".($_POST['prgid'] ? $_POST['prgid'] : 'null').",
						".($_POST['proid'] ? $_POST['proid'] : 'null').",
						".($_POST['escid'] ? $_POST['escid'] : 'null').",
						".($_POST['slcclassificacao'] ? "'".$_POST['slcclassificacao']."'" : 'null').",
						".($_POST['slcdemandarepetida'] ? "'".$_POST['slcdemandarepetida']."'" : 'null').",
						".($_POST['slcrefsolicrepetida'] ? "'".$_POST['slcrefsolicrepetida']."'" : 'null').",
						--'{$_POST['slcclassificacao']}',
                                                ".($_REQUEST['slcpalavraschave'] ? "'".$_REQUEST['slcpalavraschave']."'" : 'null').",
                                                ".($_REQUEST['classificacaoconteudo'] ? "'".$_REQUEST['classificacaoconteudo']."'" : 'null').",
                                                ".($_REQUEST['slctelefone'] ? "'".subistituiCaracteres($_REQUEST['slctelefone'])."'" : 'null')."
					) 
				returning slcid;";
		$slcid = $db->pegaUm($sql);
	}
	
	// Cria sessão da solicitação
	if($slcid){
		$_SESSION['sic']['slcid'] = $slcid;
	}
	
	// Insere atualiza programa
	if($_POST['prgid']){
		if($db->pegaUm("select prgid from sic.solicitacaoprograma where slcid = {$slcid}")){
			$sql = "update sic.solicitacaoprograma set prgid = {$_POST['prgid']} where slcid = {$slcid}";
			$db->executar($sql);
		}else{
			if($db->pegaUm("select prgid from sic.solicitacaoprograma where prgid = {$_POST['prgid']}")){
				$sql = "insert into sic.solicitacaoprograma (prgid, slcid) values ({$_POST['prgid']}, {$slcid})";
				$db->executar($sql);
			}
		}
		
	}
	
	if($_REQUEST['slctipopessoa'] == 'J'){
		
		// Insere tipo de instituição
		if(count($_REQUEST['tpiid'])){
			
			$sql = "delete from sic.solicitacaotipoinst where slcid = {$slcid}";
			$db->executar($sql);
			
			$sql = '';
			foreach($_REQUEST['tpiid'] as $tpiid){
				$sql .= "insert into sic.solicitacaotipoinst (slcid, tpiid) values ({$slcid}, {$tpiid});";
			}
			$db->executar($sql);
		}
		
		// Insere área de atuação
		if(count($_REQUEST['aatid'])){
			
			$sql = "delete from sic.solicitacaoarea where slcid = {$slcid}";
			$db->executar($sql);
			
			$sql = '';
			foreach($_REQUEST['aatid'] as $aatid){
				$sql .= "insert into sic.solicitacaoarea (slcid, aatid) values ({$slcid}, {$aatid});";
			}
			$db->executar($sql);
		}
		
	}else{
		
		// Insere escolaridade
		/*
		if(count($_REQUEST['escid'])){
			
			$sql = "delete from sic.solicitacaoescolaridade where slcid = {$slcid}";
			$db->executar($sql);
			
			$sql = '';
			foreach($_REQUEST['escid'] as $escid){
				$sql .= "insert into sic.solicitacaoescolaridade (slcid, escid) values ({$slcid}, {$escid});";
			}
			$db->executar($sql);
		}
		*/
		
		// Insere ocupação
		/*
		if(count($_REQUEST['ocpid'])){
			
			$sql = "delete from sic.solicitacaoocupacao where slcid = {$slcid}";
			$db->executar($sql);
			
			$sql = '';
			foreach($_REQUEST['ocpid'] as $ocpid){
				$sql .= "insert into sic.solicitacaoocupacao (slcid, ocpid) values ({$slcid}, {$ocpid});";
			}
			$db->executar($sql);
		}
		*/
		
	}
	
	$db->commit();
	$db->sucesso('principal/formPedidoInformacao');	
}

function atualizaDadosEntidade($post)
{
	global $db;

	$post['entnumrg'] = $post['entnumrg'] ? $post['entnumrg'] : $post['entnumdoc'];
	
	if(empty($post['entid'])){
		
		$sql = "insert into entidade.entidade
					(
						entnumcpfcnpj, 
						entnome, 
						entemail, 
						entnumrg, 
						entorgaoexpedidor, 
						entnumdddcomercial, 
						entnumramalcomercial, 
						entnumcomercial, 
						entnumdddfax,
						entnumramalfax, 
						entnumfax,
						entsig
					)
				values
					(
						'{$post['entnumcpfcnpj']}', 
						'{$post['entnome']}', 
						'{$post['entemail']}', 
						'{$post['entnumrg']}', 
						'{$post['entorgaoexpedidor']}', 
						'{$post['entnumdddcomercial']}',
						'{$post['entnumramalcomercial']}',
						'{$post['entnumcomercial']}',
						'{$post['entnumdddfax']}',
						'{$post['entnumramalfax']}',
						'{$post['entnumfax']}',
						'{$post['entsig']}'
					)
				returning entid;";
		
		$entid = $db->pegaUm($sql);
		
		if(!empty($post['endcep']) || !empty($post['estuf']) || !empty($post['muncod']) || !empty($post['endlog'])
			|| !empty($post['endnum']) || !empty($post['endcom']) || !empty($post['endbai'])){
			
			$sql = "insert into entidade.endereco
						(
							entid, 
							endcep, 
							estuf, 
							muncod, 
							endlog, 
							endnum, 
							endcom, 
							endbai
						)
					values
						(
							{$entid}, 
							'{$post['endcep']}',
							'{$post['estuf']}',
							'{$post['muncod']}',
							'{$post['endlog']}',
							'{$post['endnum']}',
							'{$post['endcom']}',
							'{$post['endbai']}'
						)";
			
			$db->executar($sql);
		}
		
	}else{	

		$arUpdate = array();
		if($post['entemail']){
			$arUpdate[] = "entemail = '{$post['entemail']}'";
		}
		if($post['entnumdddcomercial']){
			$arUpdate[] = "entnumdddcomercial = '{$post['entnumdddcomercial']}'";
		}
		if($post['entnumramalcomercial']){
			$arUpdate[] = "entnumramalcomercial = '{$post['entnumramalcomercial']}'";
		}
		if($post['entnumcomercial']){
			$arUpdate[] = "entnumcomercial = '{$post['entnumcomercial']}'";
		}
		if($post['entnumdddfax']){
			$arUpdate[] = "entnumdddfax = '{$post['entnumdddfax']}'";
		}
		if($post['entnumramalfax']){
			$arUpdate[] = "entnumramalfax = '{$post['entnumramalfax']}'";
		}
		if($post['entnumfax']){
			$arUpdate[] = "entnumfax = '{$post['entnumfax']}'";
		}
		if($post['entsig']){
			$arUpdate[] = "entsig = '{$post['entsig']}'";
		}
		
		if(count($arUpdate)){
			$sql = "update entidade.entidade set ".implode($arUpdate, ',')." where entid = ".$post['entid'].';';
			$db->executar($sql);
		}

		if(!empty($post['endcep']) || !empty($post['estuf']) || !empty($post['muncod']) || !empty($post['endlog'])
			|| !empty($post['endnum']) || !empty($post['endcom']) || !empty($post['endbai'])){
			
			if($db->pegaUm("select count(*) from entidade.endereco where entid = {$post['entid']}")){
			
				$sql = "update entidade.endereco set
							endcep 	= '{$post['endcep']}',
							estuf 	= '{$post['estuf']}',
							muncod	= '{$post['muncod']}',
							endlog	= '{$post['endlog']}',
							endnum	= '{$post['endnum']}',
							endcom	= '{$post['endcom']}',
							endbai	= '{$post['endbai']}'
						where 
							entid = {$post['entid']};";
			}else{
				
				$sql = "insert into entidade.endereco
						(
							entid, 
							endcep, 
							estuf, 
							muncod, 
							endlog, 
							endnum, 
							endcom, 
							endbai
						)
					values
						(
							{$post['entid']}, 
							'{$post['endcep']}',
							'{$post['estuf']}',
							'{$post['muncod']}',
							'{$post['endlog']}',
							'{$post['endnum']}',
							'{$post['endcom']}',
							'{$post['endbai']}'
						);";
			}
			$db->executar($sql);
		}
		
	}
	$db->commit();
	
	$entid = $entid ? $entid : $post['entid'];
	
	return $entid;		
}

$slcid = $_SESSION['sic']['slcid'] ? $_SESSION['sic']['slcid'] : $_REQUEST['slcid'];

if(!$_SESSION['sic']['slcid']){
	$_SESSION['sic']['slcid'] = $slcid; 
}

if($slcid){
	
	$sql = "select 
				sc.slcid,
				sc.tprid,
				sc.entid,
				sc.tpdid,
				sc.secid,
				sc.slcpergunta,
				trim(sc.slcclassificacao) as slcclassificacao,
				sc.slcdemandarepetida,
				trim(sc.slcrefsolicrepetida) as slcrefsolicrepetida,
				sc.slctipopessoa,
				sc.slcnomerepresentante,
				sc.slccargorepresentante,
				sc.slcnumsic,
				--sc.slccritico,
				sc.slcdtinclusao,
				sc.tpsid,
				sc.slcprioridade,
				sc.proid,
				sc.escid,
                                sc.slcpalavraschave,
                                sc.classificacaoconteudo,
                                sc.slctelefone,
				ent.entnumcpfcnpj,
				ent.entnome,
				ent.entemail,
				ent.entnumrg,
				ent.entorgaoexpedidor,				
				ent.entnumdddcomercial,
				ent.entnumramalcomercial,
				ent.entnumcomercial,
				ent.entnumdddfax,
				ent.entnumramalfax,
				ent.entnumfax,
				ent.entsig,
				ede.endlog,
				ede.endbai,
				ede.endcep,
				ede.endcom,
				ede.endnum,
				ede.estuf,
				ede.muncod,
				case when sp.prgid is not null then
						sp.prgid
					 else
					 	sc.prgid
				end as prgid,
				case when sc.unicod is not null then sc.unicod
				else sec.unicod end as unicod
			from 
				sic.solicitacao sc
			left join 
				sic.secretaria sec on sc.secid = sec.secid
			left join 
				entidade.entidade ent on ent.entid = sc.entid
			left join 
				entidade.endereco ede on ede.entid = sc.entid
			left join 
				sic.solicitacaoprograma sp on sp.slcid = sc.slcid
			where
				sc.slcid = {$slcid}
			";
	$rs = $db->pegaLinha($sql);
	
	if($rs) extract($rs);
// 	$slcclassificacao = $slccritico;
	$slctelefone=subistituiCaracteres($slctelefone);
	
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "www/includes/webservice/cpf.php";
include_once APPRAIZ . "www/includes/webservice/pj.php";

echo "<br/>";
$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Formulário para Pedido de Acesso à Informação', $titulo2);
echo "<br/>";

// Recupera dados do workflow
$docid = criaDocumento($slcid);
$esdid = pegaEstadoAtual($docid);

// Funções de abas
$arMnuid = array();
if(!$docid || in_array($esdid, array(WF_ESDID_EM_CADASTRAMENTO,WF_ESDID_DEVOLVIDO_PELA_AREA_RESP))){	
	$arMnuid[] = 11631;
}
if(!$_SESSION['sic']['slcid']){
	$arMnuid[] = 11636;
}

if(!in_array($esdid,array(WF_ESDID_RECURSO_1_INSTANCIA,
						  WF_ESDID_RECURSO_2_INSTANCIA,
						  WF_ESDID_ANALISE_1_RECURSO,
						  WF_ESDID_ANALISE_2_RECURSO,
						  WF_ESDID_FINALIZADO_1_INSTANCIA,
						  WF_ESDID_FINALIZADO_2_INSTANCIA))){
	$arMnuid[] = 11052;	
}
$abacod_tela = 57548;
// ver($arMnuid,$_SESSION['sic']['slcid']);
$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

// Verifica se pode editar
$boAtivo = 'N';
if((!$docid ||  
		in_array($esdid, array(WF_ESDID_EM_CADASTRAMENTO,WF_ESDID_DEVOLVIDO_PELA_AREA_RESP))) OR
			checkPerfil(  array(SIC_PERFIL_CADASTRADOR, SIC_PERFIL_ADMINISTRADOR, SIC_PERFIL_AREA_RESPONSAVEL, SIC_PERFIL_SUPER_USUARIO) )
){
	$boAtivo = 'S';
}elseif( checkPerfil(array(SIC_PERFIL_ORGAO_CONTROLE)) ){
	$boAtivo = 'N';
}
if($entnumcpfcnpj){
	if($slctipopessoa == 'F'){
		$entnumcpfcnpj = formatar_cpf($entnumcpfcnpj);	
	}else{
		$entnumcpfcnpj = formatar_cnpj($entnumcpfcnpj);
	}
}
?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script>
	$(function(){
		
		jQuery("[name=slcdtinclusao]").addClass("required");
		jQuery("[name=tpdid]").addClass("required");
		jQuery("[name=secid]").addClass("required");
		jQuery("[name=tpsid]").addClass("required");		
		jQuery("[name=entnome]").addClass("required");
		jQuery("[name=slcnumsic]").addClass("required");
		jQuery("[name=tprid]").addClass("required");		
		jQuery("[name=slcpergunta]").addClass("required");
		jQuery("[name=entnumcpfcnpj]").addClass("required");
		jQuery("[name=unicod]").addClass("required");
		
		jQuery("#formulario_id").validate();

		$('[name=entnumcpfcnpj]').live('blur',function(){

			if(!this.value){
				return false;
			}
			
			var entnumcpfcnpj = replaceAll(replaceAll(replaceAll(this.value, '.', ''),'-',''),'/','');

			if($('[name=slctipopessoa]:checked').val() == 'F'){
				if(!validar_cpf(entnumcpfcnpj)){
					alert('O CPF informado não é válido!');
					$('[name=entnumcpfcnpj]').val('');
					return false;
				}
			}else{
				if(!validarCnpj(entnumcpfcnpj)){
					alert('O CNPJ informado não é válido!');
					$('[name=entnumcpfcnpj]').val('');
					return false;
				}
			}
			
			$.ajax({
				url		 : 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
				type	 : 'post',
				dataType : 'json',
				data	 : 'requisicao=verificaEntidade&entnumcpfcnpj='+entnumcpfcnpj+'&tipopessoa='+$('[name=slctipopessoa]:checked').val(),
				success	 : function(e){

					var retorno = 'true';
					
					if(e.entid){
						$('[name=entid]').val(e.entid);	

						if(e.entnome)					
							$('[name=entnome]').val(e.entnome).attr({'disabled':true,'readonly':true});

						if(e.entemail)
							$('[name=entemail]').val(e.entemail).attr({'disabled':true,'readonly':true});

						if(e.estuf){
							$.ajax({
								url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
								type	: 'post',
								data	: 'requisicao=carregarMunicipios&estuf='+e.estuf,
								success	: function(res){
									$('#td_combo_municipio').html(res);
									$('[name=muncod]').val(e.muncod).attr({'disabled':true,'readonly':true});
								}
							});	
						}

						if(e.estuf)
							$('[name=estuf]').val(e.estuf).attr({'disabled':true,'readonly':true});

						if(e.endcep)		
							$('[name=endcep]').val(e.endcep).attr({'disabled':true,'readonly':true});

						if(e.endlog)						
							$('[name=endlog]').val(e.endlog).attr({'disabled':true,'readonly':true});

						if(e.endcom)
							$('[name=endcom]').val(e.endcom).attr({'disabled':true,'readonly':true});

						if(e.endbai)
							$('[name=endbai]').val(e.endbai).attr({'disabled':true,'readonly':true});

						if(e.endnum)
							$('[name=endnum]').val(e.endnum).attr({'disabled':true,'readonly':true});
												
												
					}else{

						$('[name=entid]').val('');

						if($('[name=slctipopessoa]:checked').val() == 'F'){
							var comp  = new dCPF();			
							comp.buscarDados( entnumcpfcnpj );							
							if (comp.dados.no_pessoa_rf != ''){
								$('[name=entnome]').val(comp.dados.no_pessoa_rf).attr({'disabled':true,'readonly':true});
							}else{
								retorno = 'false';
							}
						}else{
							var comp  = new dCNPJ();			
							comp.buscarDados( entnumcpfcnpj );						
							if (comp.dados.no_empresarial_rf != ''){								
								$('[name=entnome]').val(comp.dados.no_empresarial_rf).attr({'disabled':true,'readonly':true});
							}else{
								retorno = 'false';
							}
						}						
					}

					if(retorno == 'false'){
						alert('Entidade não encontrada!');
						$('[name=entnome]').attr({'disabled': false, 'readonly': false}).removeClass('disabled').addClass('normal');
					}					
				}
			});			
		});

		$('[name=endcep]').blur(function(){
			
			var endcep = replaceAll(replaceAll(this.value, '-', ''), '.', '');

			$.ajax({
				url			: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
				type		: 'post',
				data		: 'requisicao=verificarCep&endcep='+endcep,
				dataType	: 'json',
				success		: function(e){
				
					if(e.muncod){
						$('[name=endlog]').val(e.logradouro);
						$('[name=endbai]').val(e.bairro);
						$('[name=estuf]').val(e.estado).attr({'disabled':true});
						$.ajax({
							url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
							type	: 'post',
							data	: 'requisicao=carregarMunicipios&estuf='+e.estado,
							success	: function(res){
								$('#td_combo_municipio').html(res);
								$('[name=muncod]').val(e.muncod).attr({'disabled':true});
							}
						});						
					}
					
				}
			});
		});
		
		$('[name=slctipopessoa]').change(function(){
			
			if(this.value == 'J'){
				
				$('#td_label_entnome').html('Razão Social');
				$('#td_label_entnumcpfcnpj').html('CNPJ');
				$('.td_label_juridica').show();
				
				var campo = '<input type="text" class="obrigatorio normal" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal(\'##.###.###/####-##\',this.value);" value="" maxlength="255" size="21" name="entnumcpfcnpj" style="text-align:left;">';
				    campo += '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
				$('#td_campo_entnumcpfcnpj').html(campo);
				
			}else{
				
				$('#td_label_entnome').html('Nome');
				$('#td_label_entnumcpfcnpj').html('CPF');
				$('.td_label_juridica').hide();
				
				var campo = '<input type="text" class="obrigatorio normal" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal(\'###.###.###-##\',this.value);" value="" maxlength="255" size="21" name="entnumcpfcnpj" style="text-align:left;">';
				    campo += '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
				$('#td_campo_entnumcpfcnpj').html(campo);
			}

		});	

		$('[name=estuf]').change(function(){
			
			$('#td_combo_municipio').html('Carregando...');			
			$.ajax({
					url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
					type	: 'post',
					data	: 'requisicao=carregarMunicipios&estuf='+this.value,
					success	: function(e){
						$('#td_combo_municipio').html(e);
					}
			});
		});

		$('#bt_novo').click(function(){
			$.ajax({
				url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
				type	: 'post',
				data	: 'requisicao=novoRegistro',
				success : function(e){
					document.location.href = 'sic.php?modulo=principal/formPedidoInformacao&acao=A';
				}
			});
		});

		$('#bt_salvar').click(function(){

			// Verifica se a data de inclusão é um dia útil
			if($('[name=slcdtinclusao]').val()){
				
				var proximoDiaUtil = replaceAll($('#proximo_dia_util_id').val(), '-', '');
				var arData 	= $('[name=slcdtinclusao]').val().split('/');
				var data 	= new Date();				
				var ano 	= data.getFullYear();
				var mes 	= data.getMonth()+1;
				var dia 	= data.getDate();

				mes = (mes <= 9) ? '0'+mes : mes;
				dia = (dia <= 9) ? '0'+dia : dia;

				$.ajax({
					url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
					type	: 'post',
					data	: 'requisicao=verificaDiaUtil&data='+arData[2]+'-'+arData[1]+'-'+arData[0], 
					success	: function(e){
						// Se não for dia útil
						if(e != 'ok'){
							arNovaData = e.split('-');
							stNovaData = arNovaData[2]+'/'+arNovaData[1]+'/'+arNovaData[0];							
														
							alert('A data deve ser um dia útil, proximo dia útil: '+stNovaData+'!');
							$('#proximo_dia_util_id, #slcdtinclusao').val(stNovaData);				
							return false;
							
						}else{

							$('#proximo_dia_util_id').val('');
						}

						dataDefault = ano+''+mes+''+dia

						var dataCorrente = proximoDiaUtil>dataDefault ? proximoDiaUtil : dataDefault;
						var dataInclusao = arData[2]+arData[1]+arData[0];

//						alert(dataCorrente+' - '+dataInclusao+' - '+proximoDiaUtil+' - '+ano+mes+dia);
						
						// Verifica se a data atual é menor que a data de inclusão
						if(dataCorrente < dataInclusao){
							
							alert('A data de inclusão deve ser menor ou igual a data atual!');
							$('[name=slcdtinclusao]').val('');
							return false;
						}
						
						$('#requisicao_id').val('salvaDados');
						$('input, select, textarea').attr({'disabled':false,'readonly':false});												
						$('#formulario_id').submit(); 		
					}
				});
			}
		});

		$('[name=tpdid]').change(function(){
			
			if(this.value == '2'){
				
				$('#tr_entnumrg').show();
				$('#tr_entnumcpfcnpj, #tr_entnumdoc, #tr_dsc_documento').hide();
				$('[name=entnumcpfcnpj], [name=entnumdoc], [name=entsig]').val('').removeClass('required');
				$('[name=entnome]').attr({'disabled':false,'readonly':false}).removeClass('disabled').addClass('normal');
				$('[name=entnumrg], [name=entorgaoexpedidor]').addClass('required');
								
			}else if(this.value == '1'){

				$('[name=entnome]').val('');
				
				if($('[name=slctipopessoa]:checked').val() == 'J'){
					$('#td_label_entnome').html('Razão Social');
					$('#td_label_entnumcpfcnpj').html('CNPJ');
				}else{
					$('#td_label_entnome').html('Nome');
					$('#td_label_entnumcpfcnpj').html('CPF');
				}
				
				$('#tr_entnumcpfcnpj').show();
				$('#tr_entnumrg, #tr_entnumdoc, #tr_dsc_documento').hide();
				$('[name=entnumrg], [name=entnumdoc], [name=entorgaoexpedidor], [name=entnumcpfcnpj], [name=entsig]').val('').removeClass('required');
				$('[name=entnome]').attr({'disabled':true,'readonly':true}).removeClass('normal').addClass('disabled');
				$('[name=entnumcpfcnpj]').addClass('required');

			}else if(this.value == '7'){

				$('#tr_entnumrg, #tr_entnumdoc, #tr_entnumcpfcnpj, #tr_dsc_documento').hide();
				$('[name=entnome]').attr({'disabled':false,'readonly':false}).removeClass('disabled').addClass('normal');
				$('[name=entnumrg], [name=entnumdoc], [name=entorgaoexpedidor], [name=entnumcpfcnpj], [name=entsig]').val('').removeClass('required');

			}else if(this.value == '6'){
					$('#tr_dsc_documento, #tr_entnumdoc').show();
					$('#tr_entnumrg, #tr_entnumcpfcnpj').hide();
					$('[name=entnome]').attr({'disabled':false,'readonly':false}).removeClass('disabled').addClass('normal');
					$('[name=entsig]').addClass('required');
					$('[name=entnumrg], [name=entorgaoexpedidor], [name=entnumcpfcnpj]').val('').removeClass('required');
			}else{
				
				$('#tr_entnumdoc').show();
				$('#tr_entnumrg, #tr_entnumcpfcnpj, #tr_dsc_documento').hide();
				$('[name=entnumrg], [name=entnumdoc], [name=entnumcpfcnpj], [name=entsig]').val('').removeClass('required');
				$('[name=entnome]').attr({'disabled':false,'readonly':false}).removeClass('disabled').addClass('normal');
				$('[name=entnumdoc]').addClass('required');
			}
						
		});

//		$('[name=secid]').live('change',function(){
//			
//			$('#td_combo_programa').html('Carregando...');
//			
//			$.ajax({
//				url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
//				type	: 'post',
//				data	: 'requisicao=carregaProgramas&secid='+this.value,
//				success	: function(e){
//					$('#td_combo_programa').html(e);
//				}
//			});
//						
//		});

		$('.bt_dados_opcionais, .bt_mais_menos').click(function(){
			
			$('.tr_dados_opcionais, .bt_mais_menos').toggle();

			if($('#bt_menos_dados').css('display') != 'none'){
				
				if($('[name=slctipopessoa]:checked').val() == 'F'){
					//$('#tr_escolaridade, #tr_ocupacao').show();
					$('#tr_tipo_instituicao, #tr_area_atuacao').hide();
				}else{
					//$('#tr_escolaridade, #tr_ocupacao').hide();
					$('#tr_tipo_instituicao, #tr_area_atuacao').show();
				}
			}else{
				
				$('#tr_escolaridade, #tr_ocupacao, #tr_tipo_instituicao, #tr_area_atuacao').hide();
			}
		});

		$('[name=entnumrg], [name=entnumdoc]').blur(function(){

			var entnumrg = this.value;
			var campo = this.name;
			
			$.ajax({
				url		 : 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
				type	 : 'post',
				data	 : 'requisicao=verificaEntidade&entnumrg='+entnumrg+'&tipo='+campo,
				dataType : 'json',
				success	 : function(e){
				
					if(e.entid){
	
						$('[name=entid]').val(e.entid);	
	
						if(e.entnome)					
							$('[name=entnome]').val(e.entnome).attr({'disabled':true,'readonly':true});
	
						if(e.entemail)
							$('[name=entemail]').val(e.entemail).attr({'disabled':true,'readonly':true});

						if(e.entorgaoexpedidor && campo == 'entnumrg')
							$('[name=entorgaoexpedidor]').val(e.entorgaoexpedidor).attr({'disabled':true,'readonly':true});
						

						if(e.estuf){
							$.ajax({
								url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
								type	: 'post',
								data	: 'requisicao=carregarMunicipios&estuf='+e.estuf,
								success	: function(res){
									$('#td_combo_municipio').html(res);
									$('[name=muncod]').val(e.muncod).attr({'disabled':true,'readonly':true});
								}
							});	
						}

						if(e.estuf)
							$('[name=estuf]').val(e.estuf).attr({'disabled':true,'readonly':true});

						if(e.endcep)		
							$('[name=endcep]').val(e.endcep).attr({'disabled':true,'readonly':true});

						if(e.endlog)						
							$('[name=endlog]').val(e.endlog).attr({'disabled':true,'readonly':true});

						if(e.endcom)
							$('[name=endcom]').val(e.endcom).attr({'disabled':true,'readonly':true});

						if(e.endbai)
							$('[name=endbai]').val(e.endbai).attr({'disabled':true,'readonly':true});

						if(e.endnum)
							$('[name=endnum]').val(e.endnum).attr({'disabled':true,'readonly':true});
					}
				}
			});
		});

		$('[name=unicod]').change(function(){
			$.ajax({
				url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
				type	: 'post',
				data	: 'requisicao=carregarSecretarias&unicod='+this.value,
				success	: function(e){
					$('#td_secretaria').html(e);
				}
			});			
		});
		
	});	

	function carregarSecretarias(secid)
	{
		$('#td_combo_programa').html('Carregando...');
		
		$.ajax({
			url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
			type	: 'post',
			data	: 'requisicao=carregaProgramas&secid='+secid,
			success	: function(e){
				$('#td_combo_programa').html(e);
			}
		});
	}
	
	function demRepetida(){

		tr  = document.getElementById('tr_demrepetida');
		
		if(document.getElementsByName('slcdemandarepetida')[0].checked == true){
			tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		}
		if(document.getElementsByName('slcdemandarepetida')[1].checked == true){
			tr.style.display = 'none';
		}
	}
        
        function Trim(valor){
	op1 = valor.replace(";","");
op2 = op1.replace(/^\s+|\s+$/g,"");
return op2;
}

</script>
<?php if($slctipopessoa == 'F'): ?>
	<script>
		$(function(){
			
			$('#td_label_entnome').html('Nome');
			$('#td_label_entnumcpfcnpj').html('CPF');
			$('.td_label_juridica').hide();			
			var campo = '<input type="text" value="<?php echo $entnumcpfcnpj ?>" class="obrigatorio normal" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal(\'###.###.###-##\',this.value);" value="" maxlength="255" size="21" name="entnumcpfcnpj" style="text-align:left;">';
			    campo += '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
			$('#td_campo_entnumcpfcnpj').html(campo);
			
		});
	</script>
<?php endif; ?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<form name="formulario" id="formulario_id" method="post" action="">
	<input type="hidden" name="entid" id="entid" value="<?php echo $entid ? $entid : '' ?>" />
	<input type="hidden" name="slcid" id="slcid" value="<?php echo $slcid ? $slcid : '' ?>" />	
	<input type="hidden" name="requisicao" id="requisicao_id" value="" />
	<input type="hidden" name="proximo_dia_util" id="proximo_dia_util_id" value="" />
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloCentro" colspan="2">Pedido</td>
		</tr>
		<?$perfis = pegaPerfilGeral();
		if($db->testa_superuser()||in_array(SIC_PERFIL_CADASTRADOR, $perfis)||in_array(SIC_PERFIL_ADMINISTRADOR, $perfis)){
		?>
		<tr>
			<td class="subtituloDireita">Cadastrador <?php echo SIGLA_SISTEMA; ?>:</td>
			<td>
				<?php
				if($slcid){
					$sql = "SELECT 
								us.usunome
							FROM sic.solicitacao sc
							inner join seguranca.usuario us on us.usucpf = sc.usucpfinclusao
							WHERE
								sc.slcid = {$slcid}";
					
					$cpfsimec = $db->pegaUm($sql);
				} 
				echo $cpfsimec
				?>
			</td>
		</tr>	
		<?}?>	
		<tr>
			<td class="subtituloDireita">Data do pedido</td>
			<td>
				<?php
				if($boAtivo == 'S'){
					$slcdtinclusao = $slcdtinclusao ? formata_data($slcdtinclusao) : date('d/m/Y');
				}else{
					$slcdtinclusao = $slcdtinclusao ? $slcdtinclusao : date('d/m/Y');
				} 
				echo campo_data('slcdtinclusao', 'S', $boAtivo, '', 'N');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Unidade</td>
			<td>
				<?php 
				$sql = "
					Select 	unicod as codigo, 
							unidsc as descricao
					From public.unidade
					Inner Join sic.secretaria using(unicod)
					group by 1,2
					order by 2
				";
				
				$db->monta_combo('unicod', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>
        <tr>
            <td class="subtituloDireita">Prioridade</td>
            <td>
                <input type="radio" name="slcprioridade" id="slcprioridade_A" value="A" <?php echo $boAtivo == 'N' ? 'disabled="disabled"' : '' ?><?php echo $slcprioridade == 'A' ? 'checked' :  ''; ?>/>&nbsp;Alta&nbsp;
                <input type="radio" name="slcprioridade" id="slcprioridade_B" value="B" <?php echo $boAtivo == 'N' ? 'disabled="disabled"' : '' ?><?php echo $slcprioridade == 'B' ? 'checked' :  '' ; ?>/>&nbsp;Baixa&nbsp;
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
		<tr>
			<td class="subtituloDireita">Secretaria</td>
			<td id="td_secretaria">
				<?php 
				$sql = "select
							secid as codigo, 
							secdsc as descricao
						from 
							sic.secretaria
						order by 
							secdsc";
				$db->monta_combo('secid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Programa</td>
			<td id="td_combo_programa">
				<?php 
				$sql = "select
							prgid as codigo, 
							prgdsc as descricao
						from 
							sic.programa
						order by 
							prgdsc";
				$db->monta_combo('prgid', array(), $boAtivo, 'Selecione...', '', '', '', '', 'N');
				?>
			</td>
		</tr>
		<tr>
			<td width="180" class="subtituloDireita">Solicitação</td>
			<td>
				<?php 
				$sql = "select
							tpsid as codigo, 
							tpsdsc as descricao
						from 
							sic.tiposolicitacao
						order by 
							tpsdsc";
				$db->monta_combo('tpsid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>
		<tr>
			<td width="180" class="subtituloDireita">Forma de resposta</td>
			<td>
				<?php 
				$sql = "select
							tprid as codigo, 
							tprdsc as descricao
						from 
							sic.tiporesposta
						order by 
							tprdsc";
				$db->monta_combo('tprid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Número SIC</td>
			<td>
				<?php
				echo campo_texto('slcnumsic', 'S', $boAtivo, '', 15, 20, '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Tipo de documento</td>
			<td>
				<?php 
				$sql = "select 
							tpdid as codigo,
							tpddsc as descricao
						from 
							sic.tipodocumento
						order by
							tpddsc";
				$tpdid = $tpdid ? $tpdid : 1;
				$db->monta_combo('tpdid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'S');
				?>
			</td>		
		</tr>
		<tr id="tr_dsc_documento" style="display:none;">
			<td class="subtituloDireita">Descrição do documento</td>
			<td>
				<?php
				echo campo_texto('entsig', 'S', $boAtivo, '', 50, 50, '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Pessoa</td>
			<td>
				<input type="radio" name="slctipopessoa" id="slctipopessoa_f" value="F" <?php echo $boAtivo == 'N' ? 'disabled="disabled"' : '' ?><?php echo $slctipopessoa == 'F' ? 'checked' : ''; ?>/>&nbsp;Física&nbsp;
				<input type="radio" name="slctipopessoa" id="slctipopessoa_j" value="J" <?php echo $boAtivo == 'N' ? 'disabled="disabled"' : '' ?><?php echo $slctipopessoa == 'J' ? 'checked' : '' ; ?>/>&nbsp;Jurídica&nbsp;			
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>		
		</tr>
		<tr id="tr_entnumdoc" style="display:none;">
			<td id="td_label_outrodoc" class="subtituloDireita">Número do documento</td>
			<td>
				<?php
				$entnumdoc = $entorgaoexpedidor ? '' : $entnumrg;
				echo campo_texto('entnumdoc', 'S', $boAtivo, '', 12, 40, '', '');				
				?>
			</td>
		</tr>	 
		<tr id="tr_entnumrg" style="display:none;">
			<td class="subtituloDireita">RG</td>
			<td>
				<?php
				$entnumrg = $entorgaoexpedidor ? $entnumrg : '';
				echo campo_texto('entnumrg', 'S', $boAtivo, '', 12, 40, '', '');
				echo "&nbsp;Orgão expedidor&nbsp;";
				echo campo_texto('entorgaoexpedidor', 'S', $boAtivo, '', 6, 10, '', '');
				?>
			</td>
		</tr>
		<tr id="tr_entnumcpfcnpj">
			<td id="td_label_entnumcpfcnpj" class="subtituloDireita">CPF</td>
			<td id="td_campo_entnumcpfcnpj">
				<?php
				echo campo_texto('entnumcpfcnpj', 'S', $boAtivo, '', 20, 255, '###.###.###-##', ''); 
				?>
			</td>
		</tr>	
		<tr>
			<td id="td_label_entnome" class="subtituloDireita">Nome</td>
			<td>
				<?php
				echo campo_texto('entnome', 'S', 'N', '', 70, 255, '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">E-mail</td>
			<td>
				<?php
				echo campo_texto('entemail', 'N', $boAtivo, '', 70, 99, '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="180">Escolaridade</td>
			<td>
				<?php 
				$sql = "select 
							escid as codigo,
							escdsc as descricao
						from 
							sic.escolaridade
						where escstatus = 'A'
						order by
							escdsc";
				$db->monta_combo('escid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'N');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="180">Profissão</td>
			<td>
				<?php 
				$sql = "select 
							proid as codigo,
							prodescricao as descricao
						from 
							sic.profissao
						where prostatus = 'A'
						order by
							prodescricao";
				$db->monta_combo('proid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'N');
				?>
			</td>
		</tr>
		<tr class="td_label_juridica" style="display:none;">
			<td class="subtituloDireita">Nome do Representante</td>
			<td>
				<?php
				echo campo_texto('slcnomerepresentante', 'N', $boAtivo, '', 70, 255, '', ''); 
				?>
			</td>
		</tr>
		<tr class="td_label_juridica" style="display:none;">
			<td class="subtituloDireita">Cargo do Representante</td>
			<td>
				<?php
				echo campo_texto('slccargorepresentante', 'N', $boAtivo, '', 70, 255, '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="180">Pergunta</td>
			<td>
				<?php 
				echo campo_textarea('slcpergunta', 'S', $boAtivo, '', 74, 10, 4000);
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="180">Classificação?</td>
			<td>
				<input type="radio" name="slcclassificacao" id="slcclassificacao_vd" value="verde" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?> <?php echo $slcclassificacao == 'verde' ? 'checked' : '' ?>/>&nbsp;<img src="../imagens/av_verde.gif" />&nbsp;
				<input type="radio" name="slcclassificacao" id="slcclassificacao_am" value="amarelo" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?> <?php echo $slcclassificacao == 'amarelo' ? 'checked' : '' ?>/>&nbsp;<img src="../imagens/av_amarelo.gif" />&nbsp;
				<input type="radio" name="slcclassificacao" id="slcclassificacao_vm" value="vermelho" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?> <?php echo $slcclassificacao == 'vermelho' ? 'checked' : '' ?>/>&nbsp;<img src="../imagens/av_vermelho.gif" />&nbsp;
			</td>
		</tr>
                <tr>
			<td class="SubTituloDireita">Este pedido de acesso ou sua respectiva resposta contém informações sujeitas a restrição de acesso, conforme previsto na Lei 12.527/2011?</td>
			<td>
				<input type="radio" name="classificacaoconteudo" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?>  value="t"  <?php echo $classificacaoconteudo == 't' ? 'checked' : '' ?>> Sim
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="radio" name="classificacaoconteudo" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?>  value="f"  <?php echo ($classificacaoconteudo == 'f' ) ? 'checked' : '' ?>> Não
			</td>
		</tr>
                <tr>
			<td class="SubTituloDireita">Palavras-Chave</td>
			<td>
                               <?php echo campo_texto('slcpalavraschave', 'N', $boAtivo, '', 150, 1000, '', '','','','','','');?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Demanda Repetida?</td>
			<td>
				<input type="radio" name="slcdemandarepetida" value="t" onclick="demRepetida();" <?php echo $slcdemandarepetida == 't' ? 'checked' : '' ?>> Sim
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="radio" name="slcdemandarepetida" value="f" onclick="demRepetida();" <?php echo ($slcdemandarepetida == 'f' ) ? 'checked' : '' ?>> Não
			</td>
		</tr>
		<tr id="tr_demrepetida" style="display:<?if($slcdemandarepetida == 'f' || $slcdemandarepetida == '') echo 'none;';?>">
			<td  align='right' class="SubTituloDireita">Identificador das demandas repetidas:</td>
			<td><?= campo_textarea('slcrefsolicrepetida', 'S ', 'S', '', 80, 5, 300); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita"></td>
			<td>
				<img src="../imagens/mais.gif"  class="bt_mais_menos" id="bt_mais_dados" style="cursor:pointer;" />
				<img src="../imagens/menos.gif" class="bt_mais_menos" id="bt_menos_dados" style="display:none;cursor:pointer;"/>
				&nbsp;
				<a href="javascript:void(0)" class="bt_dados_opcionais">
					Mostrar/Ocultar dados opcionais
				</a>
			</td>
		</tr>
		<tr class="tr_dados_opcionais" style="display:none">
			<td width="180" class="subtituloDireita">Telefone comercial</td>
			<td>
				<?php
				$entnumcomercial = $entnumcomercial ? mascaraglobal($entnumcomercial, '####-####') : '';
				?>
				DDD&nbsp;<?php echo campo_texto('entnumdddcomercial', 'N', $boAtivo, '', 4, 3, '###', ''); ?>
				&nbsp;Número&nbsp;
				<?php echo campo_texto('entnumcomercial', 'N', $boAtivo, '', 12, 10, '####-####', ''); ?>
				&nbsp;Ramal&nbsp;
				<?php echo campo_texto('entnumramalcomercial', 'N', $boAtivo, '', 5, 4, '####', ''); ?>							
			</td>
		</tr>
		<tr class="tr_dados_opcionais" style="display:none">
			<td width="180" class="subtituloDireita">Fax</td>
			<td>
				<?php
				$entnumfax = $entnumfax ? mascaraglobal($entnumfax, '####-####') : ''; 
				?>				
				DDD&nbsp;<?php echo campo_texto('entnumdddfax', 'N', $boAtivo, '', 4, 3, '###', ''); ?>
				&nbsp;Número&nbsp;
				<?php echo campo_texto('entnumfax', 'N', $boAtivo, '', 12, 10, '####-####', ''); ?>
				&nbsp;Ramal&nbsp;
				<?php echo campo_texto('entnumramalfax', 'N', $boAtivo, '', 5, 4, '####', ''); ?>				
			</td>
		</tr>
		<tr id="tr_tipo_instituicao" style="display:none">
			<td class="subtituloDireita">Tipo de instituição</td>
			<td>				
				<?php 
				$marcados = array();
				$sql = "select 
							tpiid as codigo,
							tpidsc as descricao 
						from 
							sic.tipoinstituicao 
						order by 
							tpiid";
				
				$rs = $db->carregar($sql);
				$colunaTpiid = round(count($rs)/3);
				
				if($_SESSION['sic']['slcid'])
					$marcados = $db->carregarColuna("select tpiid from sic.solicitacaotipoinst where slcid = {$_SESSION['sic']['slcid']}");
					
				//$db->monta_checkbox('tpiid[]', $sql, $marcados);
				
				if($rs):
					foreach($rs as $k => $dados){
						if($k <= $colunaTpiid){
							$colunaTpiid1 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="tpiid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}elseif($k > $colunaTpiid && $k <= $colunaTpiid+$colunaTpiid){
							$colunaTpiid2 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="tpiid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}else{
							$colunaTpiid3 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="tpiid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}
					}
					?>	
					<table>
						<tr>
							<td valign="top" align="left"><?php echo $colunaTpiid1 ?></td>
							<td valign="top" align="left"><?php echo $colunaTpiid2 ?></td>
							<td valign="top" align="left"><?php echo $colunaTpiid3 ?></td>
						</tr>
					</table>
				<?php endif; ?>			
			</td>
		</tr>
		<tr id="tr_area_atuacao" style="display:none">
			<td class="subtituloDireita">Área de atuação</td>
			<td>
				<?php 
				$sql = "select 
							aatid as codigo,
							aatdsc as descricao 
						from 
							sic.areaatuacao 
						order by 
							aatid";
				
				$rs = $db->carregar($sql);				
				$colunaAatid = round(count($rs)/3);
				
				if($_SESSION['sic']['slcid'])
					$marcados = $db->carregarColuna("select aatid from sic.solicitacaoarea where slcid = {$_SESSION['sic']['slcid']}");
													
				//$db->monta_checkbox('aatid[]', $sql, $marcados);
				
				if($rs):
					foreach($rs as $k => $dados){
						if($k <= $colunaAatid){
							$colunaAatid1 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="aatid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}elseif($k > $colunaAatid && $k <= $colunaAatid+$colunaAatid){
							$colunaAatid2 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="aatid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}else{
							$colunaAatid3 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="aatid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}
					}
					?>
					<table>
						<tr>
							<td valign="top" align="left"><?php echo $colunaAatid1 ?></td>
							<td valign="top" align="left"><?php echo $colunaAatid2 ?></td>
							<td valign="top" align="left"><?php echo $colunaAatid3 ?></td>
						</tr>
					</table>
				<?php endif; ?>				
			</td>
		</tr>
		
		<tr id="tr_escolaridade" style="display:none">
			<td class="subtituloDireita">Escolaridade (completa)</td>
			<td>				
				<?php 
				$marcados = array();
				$sql = "select 
							escid as codigo,
							escdsc as descricao 
						from 
							sic.escolaridade 
						order by 
							escid";
				
				$rs = $db->carregar($sql);
				$colunaEscid = round(count($rs)/3);
				
				if($_SESSION['sic']['slcid'])
					$marcados = $db->carregarColuna("select escid from sic.solicitacaoescolaridade where slcid = {$_SESSION['sic']['slcid']}");
					
				if($rs):
					foreach($rs as $k => $dados){
						if($k <= $colunaEscid){
							$colunaEscid1 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="escid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}elseif($k > $colunaEscid && $k <= $colunaEscid+$colunaEscid){
							$colunaEscid2 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="escid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}else{
							$colunaEscid3 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="escid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}
					}
					?>	
					<table>
						<tr>
							<td valign="top" align="left"><?php echo $colunaEscid1 ?></td>
							<td valign="top" align="left"><?php echo $colunaEscid2 ?></td>
							<td valign="top" align="left"><?php echo $colunaEscid3 ?></td>
						</tr>
					</table>
				<?php endif; ?>			
			</td>
		</tr>
		<tr id="tr_ocupacao" style="display:none">
			<td class="subtituloDireita">Escolaridade (completa)</td>
			<td>				
				<?php 
				$marcados = array();
				$sql = "select 
							ocpid as codigo,
							ocpdsc as descricao 
						from 
							sic.ocupacao 
						order by 
							ocpid";
				
				$rs = $db->carregar($sql);
				$colunaOcpid = round(count($rs)/3);
				
				if($_SESSION['sic']['slcid'])
					$marcados = $db->carregarColuna("select ocpid from sic.solicitacaoocupacao where slcid = {$_SESSION['sic']['slcid']}");
					
				if($rs):
					foreach($rs as $k => $dados){
						if($k <= $colunaOcpid){
							$colunaOcpid1 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="ocpid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}elseif($k > $colunaOcpid && $k <= $colunaOcpid+$colunaOcpid){
							$colunaOcpid2 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="ocpid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}else{
							$colunaOcpid3 .= '<input type="checkbox" value="'.$dados['codigo'].'" name="ocpid[]" '.(array_search($dados['codigo'], $marcados) !== false ? 'checked="checked"' : '').''.($boAtivo != 'S' ? 'disabled="disabled"' : '').'>'.$dados['descricao'].'<br/>';
						}
					}
					?>	
					<table>
						<tr>
							<td valign="top" align="left"><?php echo $colunaOcpid1 ?></td>
							<td valign="top" align="left"><?php echo $colunaOcpid2 ?></td>
							<td valign="top" align="left"><?php echo $colunaOcpid3 ?></td>
						</tr>
					</table>
				<?php endif; ?>			
			</td>
		</tr>
		
		<tr>
			<td class="subtituloCentro" colspan="2">Logradouro</td>
		</tr>
		<tr>
			<td class="subtituloDireita">CEP</td>
			<td>
				<?php
				$endcep = $endcep ? formata_cep($endcep) : '';
				echo campo_texto('endcep', 'N', $boAtivo, '', 10, 40, '##.###-###', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">UF</td>
			<td>
				<?php
				$sql = "select
							estuf as codigo,
							estdescricao as descricao
						from 
							territorios.estado
						order by
							estdescricao";
				$db->monta_combo('estuf', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'N');			 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município</td>
			<td id="td_combo_municipio">
				<?php if($estuf && $muncod): ?>
					<script>
						$(function(){
							$.ajax({
								url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
								type	: 'post',
								data	: 'requisicao=carregarMunicipios&estuf=<?php echo $estuf ?>',
								success	: function(res){
									$('#td_combo_municipio').html(res);
									$('[name=muncod]').val(<?php echo $muncod ?>);
								}
							});		
						});
					</script>
				<?php endif; ?>
				<?php			
				$db->monta_combo('muncod', array(), $boAtivo, 'Selecione...', '', '', '', '', 'N');			 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Logradouro</td>
			<td>
				<?php
				echo campo_texto('endlog', 'N', $boAtivo, '', 70, 255, '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Número</td>
			<td>
				<?php
				echo campo_texto('endnum', 'N', $boAtivo, '', 9, 9, '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Complemento</td>
			<td>
				<?php
				echo campo_texto('endcom', 'N', $boAtivo, '', 70, 255, '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Bairro</td>
			<td>
				<?php
				echo campo_texto('endbai', 'N', $boAtivo, '', 50, 90, '', ''); 
				?>
			</td>
		</tr>	
                		<tr>
			<td class="subtituloDireita">Telefone</td>
			<td>
				<?php 
// 				$slctelefone = $slctelefone ? mascaraglobal($slctelefone,'(##)#####-####') : '';
				?>
				<?php echo campo_texto('slctelefone', 'N', $boAtivo, '', 20, 25,'##-#####-####','','','','','','if(this.value.length>12){ this.value=mascaraglobal(\'##-#####-####\',this.value);}else{ this.value=mascaraglobal(\'##-####-####\',this.value);}'); ?>
					
			</td>
		</tr>	
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<?php if($boAtivo == 'S'): ?>
					<input type="button" value="Salvar" id="bt_salvar" />
					<?php if($slcid): ?>
						<input type="button" value="Novo" id="bt_novo" />	
					<?php endif; ?>
				<?php endif; ?>
				<?php if($_REQUEST['slcid']): ?>
					<input type="button" value="Voltar" onclick="javascript:document.location.href = 'sic.php?modulo=principal/listaPedidos&acao=A'"/>
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>
<?php if($docid): ?>
	<div style="position:absolute;right:80px;top:270px;">
		<?php
		$oculta = true;
		if(checkPerfil(array(SIC_PERFIL_ADMINISTRADOR))){
			$oculta = false;
		}
		wf_desenhaBarraNavegacao( $docid , array('slcid' => $slcid),  array('historico'=>$oculta) );
		?>
	</div>
<?php endif; ?>

<?php if($tpdid): ?>
	<?php if($tpdid == 2): ?>
		<script>
			$(function(){
				$('#tr_entnumrg').show();
				$('#tr_entnumcpfcnpj, #tr_entnumdoc').hide();				
	
				$('[name=entnumcpfcnpj], [name=entnumdoc]').val('').removeClass('required');
			});
		</script>
	<?php elseif($tpdid == 1): ?>
		<script>
			$(function(){
				
				if($('[name=slctipopessoa]:checked').val() == 'J'){
					$('#td_label_entnome').html('Razão Social');
					$('#td_label_entnumcpfcnpj').html('CNPJ');
				}else{
					$('#td_label_entnome').html('Nome');
					$('#td_label_entnumcpfcnpj').html('CPF');
				}
				
				$('#tr_entnumcpfcnpj').show();
				$('#tr_entnumrg, #tr_entnumdoc').hide();
	
				$('[name=entnumrg], [name=entnumdoc], [name=entorgaoexpedidor]').val('');
				$('[name=entnumcpfcnpj]').addClass('required')
				
			});
		</script>	
	<?php elseif($tpdid == 6): ?>
		<script>
			$(function(){
				$('#tr_dsc_documento, #tr_entnumdoc').show();
				$('[name=entsig]').addClass('required');
				$('#tr_entnumrg, #tr_entnumcpfcnpj').hide();
				$('[name=entnome]').attr({'disabled':false,'readonly':false}).removeClass('disabled').addClass('normal');				
				$('[name=entnumrg], [name=entorgaoexpedidor], [name=entnumcpfcnpj]').val('').removeClass('required');
			});
		</script>
	<?php else: ?>
		<script>
			$(function(){
				$('#tr_entnumdoc').show();
				$('#tr_entnumrg, #tr_entnumcpfcnpj').hide();
                //$('[name=entnumrg], [name=entnumdocc], [name=entnumcpfcnpj]').val('').removeClass('required');
                //alterando para exibir valor pois entnumdoc nessa condição será exibido
                $('[name=entnumrg], [name=entnumcpfcnpj]').val('').removeClass('required');
			});
		</script>
	<?php endif; ?>
<?php endif; ?>

<?php if($secid): ?>
	<script>
	jQuery(document).ready(function(){
		if(jQuery('[name="slctelefone"]').val().length>12){ 
			jQuery('[name="slctelefone"]').val(mascaraglobal('##-#####-####',jQuery('[name="slctelefone"]').val()));
		}else{ 
			jQuery('[name="slctelefone"]').val(mascaraglobal('##-####-####',jQuery('[name="slctelefone"]').val()));
		}
	});
		$(function(){
						
			$('#td_combo_programa').html('Carregando...');
			
			$.ajax({
				url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
				type	: 'post',
				data	: 'requisicao=carregaProgramas&secid=<?php echo $secid ?>',
				success	: function(e){
					$('#td_combo_programa').html(e);
					$('[name=prgid]').val(<?php echo $prgid ?>);
				}
			});
		});
	</script>
<?php endif;?>

<?php if($unicod): ?>
	<script>
	$(function(){
			$.ajax({
				url		: 'sic.php?modulo=principal/formPedidoInformacao&acao=A',
				type	: 'post',
				data	: 'requisicao=carregarSecretarias&unicod=<?php echo $unicod ?>',
				success	: function(e){
					$('#td_secretaria').html(e);
					$('[name=secid]').val('<?php echo $secid?>');
				}
			});			
		});
	</script>
<?php endif; ?>