<?php
	if($_SESSION['memid'] == ''){
		echo '<script>
				alert("Ocorreu um erro interno.\n O sistema irá redirecioná-lo à página inicial do módulo.");
				location.href = "pdeescola.php?modulo=melista&acao=E";
			  </script>';
		die();
	}

	ini_set("memory_limit","1024M"); 
	set_time_limit(0);
	
	include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	include_once APPRAIZ . "pdeescola/classes/controle/CampoExternoControle.class.inc";
	include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Montatabela.class.inc";
	
	if( $_REQUEST['requisicao'] ){
		$_REQUEST['requisicao']($_REQUEST);
		die();
	}

	if( $_POST['salvar_questionario'] && $_POST['identExterno'] ){
		$obMontaEx = new CampoExternoControle();
		$obMontaEx->salvar();
	}
	
	if( $_POST['salvar_questionario'] && $_POST['idTabela'] ){
		$obMonta = new Montatabela();
		$obMonta->salvar();
	}

	#Atualizar Combo Atividade.
	function atualizaComboAtividade( $request ){
		global $db;
		
		extract($request);
	
		if($mtmid != ''){
			$mtmid = " and mtmid = ".$mtmid;
		}else{
			$mtmid = '';
		}
	
		$sql = " select mtaid as codigo, upper(mtadescricao) as descricao from pdeescola.metipoatividade where mtaanoreferencia = 2013 $mtmid;";
		
		if($posicao != ''){
			echo $db->monta_combo('atividade_'.$posicao, $sql, 'S', 'Selecione Atividade...', '', '', '', 200, 'N', 'atividade_'.$posicao, '', '', '');
		}else{
			echo $db->monta_combo('atividade', $sql, 'S', 'Selecione Atividade...', '', '', '', 170, 'N', 'atividade', '', '', '');
		}
	}

	#Gera do Combo do Representante.
	function atualizaComboRepresentante($request){
		global $db;
		
		extract($request);
		
		if($tipo_rep == 'seguimento_1'){
			$id_combo = 'seguimento_1';
		}elseif($tipo_rep == 'seguimento_2'){
			$id_combo = 'seguimento_2';
		}elseif($tipo_rep == 'seguimento_3'){
			$id_combo = 'seguimento_3';
		}
		
		$arr = Array(
				Array('codigo'=>1, 'descricao'=>'Diretor'),
				Array('codigo'=>2, 'descricao'=>'Coodenador Pedagógico'),
				Array('codigo'=>3, 'descricao'=>'Associação de Pais'),
				Array('codigo'=>4, 'descricao'=>'Representante dos Professores'),
				Array('codigo'=>5, 'descricao'=>'Funcionário da Escola'),
				Array('codigo'=>6, 'descricao'=>'Membro do Conselho Escolar'),
				Array('codigo'=>7, 'descricao'=>'Representante Estudantil'),
				Array('codigo'=>8, 'descricao'=>'Representante da Comunidade'),
		);
		echo $db->monta_combo($id_combo, $arr, 'S', 'Selecione o segmento...', '','','', '145', 'N', $id_combo, '', '', $title= null);
	}
	
	function carregarResposta($qrpid, $perid){
		global $db;
		
		if($qrpid != '' && $perid != ''){
			$sql = "
				Select 	qexid,
						queid,
						perid,
						qrpid,
						resdesc as resdec_cod,
						Case When resdesc = '1' then 'Diretor'
						     When resdesc = '2' then 'Coodenador Pedagógico'
						     When resdesc = '3' then 'Associação de Pais'
						     When resdesc = '4' then 'Representante dos Professores'
						     When resdesc = '5' then 'Funcionário da Escola'
						     When resdesc = '6' then 'Membro do Conselho Escolar'
						     When resdesc = '7' then 'Representante Estudantil'
						     When resdesc = '8' then 'Representante da Comunidade'
						     When cast(tc.mtmid as text) = qe.resdesc Then mtmdescricao
						     When cast(ta.mtaid as text) = qe.resdesc Then mtadescricao
						   else resdesc
						end as resdesc,
						qeord
				From pdeescola.mequestionarioexterno qe
				Left Join pdeescola.metipomacrocampo tc on cast(tc.mtmid as text) = qe.resdesc and mtmanoreferencia = 2013
				Left Join pdeescola.metipoatividade ta on cast(ta.mtaid as text) = qe.resdesc and mtaanoreferencia = 2013
				Where qrpid = $qrpid and perid = $perid;
			";
			$dados = $db->carregar($sql);
		}
		return $dados;
	}
	
	function pegarMemid(){
		global $db;
		$sql_ent  = "SELECT entcodent FROM pdeescola.memaiseducacao WHERE memid = {$_SESSION['memid']}";
		$entidade = $db->pegaUm($sql_ent);

		$sql = "SELECT 	memid 
				FROM 	pdeescola.memaiseducacao 
				WHERE 	entcodent = '{$entidade}' AND memanoreferencia IN (2012);";
		$dado = $db->pegaUm($sql);
		return $dado;
	}
	

	function carregarValorPagoPDDE(){
		global $db;
		
		if($_SESSION['exercicio'] == 2013){
			$memid = $_SESSION['memid'];
		} else {
			$memid = pegarMemid();		
		}
	 	/*
		$sql = "SELECT 	TRIM(TO_CHAR(memvlrpago, '999G999G999G999D99')) AS memvlrpago
				FROM 	pdeescola.memaiseducacao 
				WHERE 	memanoreferencia = 2013 AND memstatus = 'A' AND memid = {$memid};";
		*/
		$sql = "select TRIM(TO_CHAR(sum(dshqtde), '999G999G999G999D99')) AS memvlrpago --dshcod as inep, sum(dshqtde) as totalescola 
				from painel.indicador i
				inner join painel.seriehistorica sh on sh.indid = i.indid
				inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
				inner join pdeescola.memaiseducacao me on me.entcodent = dsh.dshcod and memanoreferencia = ".($_SESSION['exercicio']-1)."
				where i.indid = 1420
				and sh.sehstatus <> 'I'
				and sh.dpeid = 1148 --2012
				and  me.entid = ".$_SESSION['meentid']."  ---entcodent
				group by dshcod";
		$dado = $db->pegaUm($sql);
		return $dado;
	}
	
	function carregarSaldoPDDE(){
		global $db;
		
		if($_SESSION['exercicio'] == 2013){
			$memid = $_SESSION['memid'];
		} else {
			$memid = pegarMemid();		
		}
	 	
		$sql = "select TRIM(TO_CHAR(memvlrsaldodisponivel, '999G999G999G999D99')) as saldo 
				from pdeescola.memaiseducacao 
				where memanoreferencia = ".($_SESSION['exercicio']-1)." 
				and entid = ".$_SESSION['meentid']."
				--and entcodent = '11000473'
				";
		$dado = $db->pegaUm($sql);
		return $dado;
	}

	function carregarDuracaoAtividades($perid,$qrpid,$queid){
		global $db;

		$sql_duracao = "SELECT 		qexid, queid, perid, qrpid, resdesc, qeord 
						FROM 		pdeescola.mequestionarioexterno 
						WHERE		queid = {$queid} AND perid = {$perid} AND qrpid = {$qrpid}
						ORDER BY	qeord";
		$dados_duracao = $db->carregar($sql_duracao);	
		return $dados_duracao;
	}		
	
	function carregarAtividadesDesenvolvidas($qrpid){

		global $db;

		if($_SESSION['exercicio'] == 2012){
			$memid = $_SESSION['memid'];
		} else {
			$memid = pegarMemid();		
		}		
		
		$sql_macro = "SELECT		mea.meaid,
									mea.mealocalizacao,
									mtm.mtmid,
									mtm.mtmdescricao,
									mta.mtaid,
									mta.mtadescricao,
									mqa.mqaid,
									mqa.mqinicio
					  FROM			pdeescola.meatividade mea
					  INNER JOIN	pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid AND mta.mtasituacao = 'A'
					  INNER JOIN 	pdeescola.metipomacrocampo mtm ON mtm.mtmid = mta.mtmid AND mtm.mtmsituacao = 'A'
					   LEFT JOIN  	pdeescola.mequestionarioexternoatividade mqa ON mqa.mtaid = mea.mtaid 
					         AND 	mtm.mtmsituacao = 'A' 
					         AND 	mqa.qrpid = {$qrpid}  
					  WHERE			mea.memid = {$memid} AND mea.meaano = 2012
					  ORDER BY		mea.meaid";
		

		$dados_macro = $db->carregar($sql_macro);
		$aryDados = array();
		
		if($dados_macro){
			foreach($dados_macro as $dados){
				$sql_quant = "SELECT		map.mpaquantidade
							  FROM			pdeescola.mealunoparticipanteatividade map
						      WHERE			map.meaid = {$dados['meaid']} AND map.sceid IN (1,2,3,4,5,6,7,8,9)
							  ORDER BY		map.sceid";
				
				$dados_quant = $db->carregar($sql_quant);
	
				$aryQuant = array();
	
				foreach($dados_quant as $quant){
					array_push($aryQuant,$quant['mpaquantidade']);
				}
				
				array_push($aryQuant,$dados['mtaid'],$dados['mtmid'],$dados['mtmdescricao'],$dados['mtadescricao'],$dados['mqinicio']);
				array_push($aryDados, $aryQuant);
			}
	}
		return $aryDados;
	}	
		
	function carregarListaGrid($qrpid){
		global $db;

		$habilitado = buscaPermissaoAcesso();
		
		if($habilitado == 'S'){
			$acao = "'<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"deletarMacrocampoAtividade('||m.qrpid||','||m.mtmid||','||m.mtaid||', this);\">' as acao,";
		}else{
			$acao = "'<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\">' as acao,";
		}
				
		$sql = "
			Select 	distinct (m.qrpid),
					".$acao."
					m.mtmid,
					m.mtaid,
					upper(mc.mtmdescricao) ||', '|| upper(ma.mtadescricao) as macroatividade,
					ano.qano1, ano.qano2, ano.qano3, ano.qano4, ano.qano5, ano.qano6, ano.qano7, ano.qano8, ano.qano9,
					Case when m.mqinicio = 'S' Then 'Sim' Else 'Não'End as mqinicio,
					ano.qano10 as total
			From pdeescola.mequestionarioexternoatividade m
			Join (
					Select 	mtmid, qano1, qano2, qano3, qano4, qano5, qano6, qano7, qano8, qano9, qano10
					From
					crosstab(
							'select mtmid, qrpid, mqresposta
				   From pdeescola.mequestionarioexternoatividade
				   where qrpid = ''".$qrpid."''
				   Order by 1,2,3')
					AS ct (mtmid integer, qano1 integer, qano2 integer, qano3 integer, qano4 integer, qano5 integer, qano6 integer, qano7 integer, qano8 integer, qano9 integer, qano10 integer )
			) as ano on ano.mtmid = m.mtmid
			Join pdeescola.metipomacrocampo mc on mc.mtmid = m.mtmid
			Join pdeescola.metipoatividade ma on ma.mtaid = m.mtaid
			Where m.qrpid = ".$qrpid."
			Order by 1
		";
		$grid = $db->carregar($sql);
		return $grid;		
	}
	
	function deletarMacrocampoAtividade($request){
		global $db;
		
		extract($request);
		
		$sql = "
			Delete From pdeescola.mequestionarioexternoatividade Where qrpid = ".$qrpid." and mtmid = ".$mtmid." and mtaid = ".$mtaid." returning mqaid;
		";
		$dados = $db->carregar($sql);
		if($dados[0][mqaid] != ''){
			$db->commit();
			echo trim("ok");
		}else{
			echo trim("Ocorreu um erro interno tente novamente mais tarde!");
		}
	}
	
	
	function carregarRespostaRecursosPDDE($qrpid, $perid){
		global $db;
	
		if($qrpid != '' && $perid != ''){
			/*
			$sql = "
				Select 	q.qexid,
					q.queid,
					q.perid,
					q.qrpid,
					trim(to_char(cast(q.resdesc as numeric), '999G999G999G999D99')) as valor,
					q.qeord
				From pdeescola.mequestionarioexterno q
				
				Where q.qrpid = $qrpid and q.perid = $perid;
			";
			*/
			$sql = "
				Select 	q.qexid,
					q.queid,
					q.perid,
					q.qrpid,
					q.resdesc as valor,
					q.qeord
				From pdeescola.mequestionarioexterno q
				
				Where q.qrpid = $qrpid and q.perid = $perid;
			";
			$dados = $db->carregar($sql);
		}
		return $dados;
	}	
	
	function carregarNomeCoordenador(){
		global $db;

		if($_SESSION['exercicio'] == 2013){
			$memid = $_SESSION['memid'];
		} else {
			$memid = pegarMemid();	
		}		

		if($memid != ''){
			$sql = "SELECT			DISTINCT e.entnome AS entnome
					FROM 			entidade.entidade e
					INNER JOIN		entidade.funcaoentidade fe ON e.entid = fe.entid
					LEFT JOIN 		entidade.funentassoc fea ON fea.fueid = fe.fueid
					INNER JOIN		pdeescola.memaiseducacao me ON fea.entid = me.entid
					LEFT JOIN 		entidade.endereco ed ON ed.entid = e.entid
					LEFT JOIN		territorios.municipio m ON ed.muncod = m.muncod
					WHERE 			fe.fuestatus = 'A' AND fe.funid = 41 AND e.entstatus = 'A' AND me.memid = {$memid}
					ORDER BY 		entnome";
			$dados = $db->pegaLinha($sql);
			return $dados['entnome'];
		}
	}

	#Busca qrpid relacionado com memid ou entid. 
	function pegaQrpidMaisEdFinan(){
		global $db;
		
		$queid = QUESTIONARIO_MONIT_FISICO_FINANC;

		if($_SESSION['exercicio'] == 2013){
			$memid = $_SESSION['memid'];
		} else {
			$memid = pegarMemid();		
		}		
		
		if($memid == ''){
			echo "
				<script type=\"text/javascript\">
					alert('Ocorreu um problema e não é possível responder o questionário, tente novamente mais tarde!');
					//window.location = 'pdeescola.php?modulo=inicio&acao=C';
					history.back(-1);
				</script>
			";
			exit;
		}else{
			$sql = "
				Select	qp.qrpid
				From  pdeescola.pdequestionario qp
				Join questionario.questionarioresposta qr ON qr.qrpid = qp.qrpid
				Where qp.memid = ".$memid." and qr.queid = ".$queid;
			$qrpid = $db->pegaUm( $sql );
		
			if(!$qrpid){
				$sql = "
					Select ent.entnome
					From entidade.entidade ent
					Where entid = ".$_SESSION['meentid'];
				
				$titulo = $db->pegaUm( $sql );
				$arParam = array ( "queid" => $queid, "titulo" => "PDE_ESCOLA-MONITORAMENTO_FISICO_FINANCEIRO (".$titulo.")" );
				$qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
				$sql = "INSERT INTO pdeescola.pdequestionario (qrpid, memid) VALUES (".$qrpid.", ".$memid.");";
				$db->executar( $sql );
				$db->commit();
			}
			return $qrpid;
		}
	}
	
	function buscaPermissaoAcesso(){
		global $db;

		$habilitado = 'N';
		
		$perfis = arrayPerfil();
		
		if( !$db->testa_superuser() ){
			foreach( $perfis as $perfil ){
				if( $perfil == PDEESC_PERFIL_CAD_MAIS_EDUCACAO){
					$habilitado = 'S';
					break;
				}
			}
		}else{
			$habilitado = 'S';
		}
		return $habilitado;
	}

	function formata_valor_sql($valor){
		$valor = str_replace('.', '', $valor);
		$valor = str_replace(',', '.', $valor);
		return $valor;
	}

	include APPRAIZ."includes/cabecalho.inc";
	echo'<br>';
	
	echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/me_questionario_fisico_financeiro&acao=A");
	
	$titulo = "Mais Educação";
	$subtitulo = "Questionário Monitoramento Físico-Financeiro";

	monta_titulo($titulo, $subtitulo);
	
	$sql = "SELECT 
				true
			FROM 
				pdeescola.memaiseducacao m1
			inner join pdeescola.memaiseducacao m2 ON m2.entcodent = m1.entcodent
			WHERE 
				m1.mamescolaaberta = 't'
				AND m1.memanoreferencia = 2012
				AND m1.memstatus = 'A'
				AND m2.memid = '".$_SESSION['memid']."'";
	$_testa_perg_14 = $db->pegaUm($sql); 
//ver($sql);
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	jQuery.noConflict();
	
	jQuery(document).ready(function(){
		<? if( $_testa_perg_14 != 't' ){ ?>
		jQuery('[id*="sarvore"]:contains("14 ")').parent().next().remove();
		jQuery('[id*="sarvore"]:contains("14 ")').parent().remove();
		
		jQuery('#telacentral').bind('DOMNodeInserted DOMNodeRemoved', function(event) {
			jQuery('[id*="sarvore"]:contains("14 ")').parent().next().remove();
			jQuery('[id*="sarvore"]:contains("14 ")').parent().remove();
			if( jQuery('[id*="sarvore"]:contains("A prefeitura/secretaria oferece contrapartida?")').html().indexOf('<b>') > 0 ){
				jQuery('#salvar_proximo').remove();
				jQuery('#proximo').remove();
			}
		});
		<? } ?>
	});

	function calcularCapitalCusteio(){
		/*
		jQuery('#valor_2').keyup(function(){
			var valor_1 = $('#valor_1').val();		
			var valor_2 = $('#valor_2').val();
			var valor_3 = $('#valor_3').val();
			var valor_4 = $('#valor_4').val();

			valor_1 = replaceAll(replaceAll(valor_1, ".", ""), ",", ".");
			valor_2 = replaceAll(replaceAll(valor_2, ".", ""), ",", "."); 
			valor_3 = replaceAll(replaceAll(valor_3, ".", ""), ",", ".");
			valor_4 = replaceAll(replaceAll(valor_4, ".", ""), ",", ".");
			
			var saldo =  parseFloat( valor_1 ) - ( parseFloat( valor_2 ) + parseFloat( valor_3 ) + parseFloat( valor_4));
			
			
			if(parseFloat(saldo) < parseFloat(0) ){
				$('#valor_5').css("color", "red");
				$('#valor_5').val('-'+mascaraglobal("[.###],##", saldo.toFixed(2)));
			}else{
				$('#valor_5').css("color", "green");
				$('#valor_5').val(mascaraglobal("[.###],##", saldo.toFixed(2)));
			}
			
			
		});
		*/
		
		jQuery('#valor_3').blur(function(){		
			//var valor_1 = $('#valor_1').val();		
			var valor_2 = $('#valor_2').val();
			var valor_3 = $('#valor_3').val();
			
			valor_2 = replaceAll(replaceAll(valor_2, ".", ""), ",", ""); 
			valor_3 = replaceAll(replaceAll(valor_3, ".", ""), ",", "");
			
			if( parseFloat(valor_1) > 0 || parseFloat(valor_2) > 0  ){
				if($('#valor_4').val()==""){
					$('#valor_4').val(mascaraglobal('[.###],##',parseFloat(valor_2) - parseFloat(valor_3)));
				}
			}
			
			
			var valor_4 = $('#valor_4').val();
			/*
			if(valor_3.length > 0 && valor_3.length < 4){
				alert("Valor Capital inválido. Ex.: 0,00");
				$('#valor_3').val("");
			}*/
			

			//valor_1 = replaceAll(replaceAll(valor_1, ".", ""), ",", ".");
			valor_4 = replaceAll(replaceAll(valor_4, ".", ""), ",", "");
			
			if(!valor_2) valor_2 = 0;
			if(!valor_3) valor_3 = 0;
			if(!valor_4) valor_4 = 0;
			
			if( parseFloat(valor_4) > 0 || parseFloat(valor_3) > 0 ){
				if($('#valor_3').val() && $('#valor_4').val()){
					if( parseFloat(valor_2) != ( parseFloat(valor_3) + parseFloat(valor_4) ) ){
						alert("A soma dos valores Capital e Custeio deve ser igual ao Saldo 31.12.2012!");
						if(parseFloat(valor_4) > parseFloat(valor_2)) $('#valor_4').val("");
						$('#valor_3').val("");
					}
				}
			}
			/*
			var saldo =  parseFloat( valor_1 ) - ( parseFloat( valor_2 ) + parseFloat( valor_3 ) + parseFloat( valor_4));
			
			if(parseFloat(saldo) < parseFloat(0) ){
				$('#valor_5').css("color", "red");
				$('#valor_5').val('-'+mascaraglobal("[.###],##", saldo.toFixed(2)));
			}else{
				$('#valor_5').css("color", "green");
				$('#valor_5').val(mascaraglobal("[.###],##", saldo.toFixed(2)));
			}
			*/
			
		});

		jQuery('#valor_4').blur(function(){	
			//var valor_1 = $('#valor_1').val();		
			var valor_2 = $('#valor_2').val();
			var valor_4 = $('#valor_4').val();
			
			valor_2 = replaceAll(replaceAll(valor_2, ".", ""), ",", ""); 
			valor_4 = replaceAll(replaceAll(valor_4, ".", ""), ",", "");
			
			if( parseFloat(valor_1) > 0 || parseFloat(valor_2) > 0  ){
				if($('#valor_3').val()==""){
					$('#valor_3').val(mascaraglobal('[.###],##',parseFloat(valor_2) - parseFloat(valor_4)));
				}
			}

			var valor_3 = $('#valor_3').val();
			/*
			if(valor_4.length > 0 && valor_4.length < 4){
				alert("Valor C l inválido. Ex.: 0,00");
				$('#valor_4').val("");
			}
			*/

			//valor_1 = replaceAll(replaceAll(valor_1, ".", ""), ",", ".");
			valor_3 = replaceAll(replaceAll(valor_3, ".", ""), ",", "");
			
			if(!valor_2) valor_2 = 0;
			if(!valor_3) valor_3 = 0; 
			if(!valor_4) valor_4 = 0;
			
			if( parseFloat(valor_4) > 0 || parseFloat(valor_3) > 0 ){
				if($('#valor_3').val() && $('#valor_4').val()){
					if( parseFloat(valor_2) != ( parseFloat(valor_3) + parseFloat(valor_4) ) ){
						alert("A soma dos valores Capital e Custeio deve ser igual ao Saldo 31.12.2012!");
						if(parseFloat(valor_3) > parseFloat(valor_2)) $('#valor_3').val("");
						$('#valor_4').val("");
					}
				}
			}
			/*
			var saldo =  parseFloat( valor_1 ) - ( parseFloat( valor_2 ) + parseFloat( valor_3 ) + parseFloat( valor_4));
			
			if(parseFloat(saldo) < parseFloat(0) ){
				$('#valor_5').css("color", "red");
				$('#valor_5').val('-'+mascaraglobal("[.###],##", saldo.toFixed(2)));
			}else{
				$('#valor_5').css("color", "green");
				$('#valor_5').val(mascaraglobal("[.###],##", saldo.toFixed(2)));
			}
			*/
		});

		
	}
	
	function atualizaComboAtividade( mtmid, posicao ){
		var posicao = posicao.replace('macrocampo_','');
		jQuery.ajax({
			type: "POST",
			url: "pdeescola.php?modulo=meprincipal/me_questionario_fisico_financeiro&acao=A",
			data: "requisicao=atualizaComboatividade&mtmid="+mtmid+"&posicao="+posicao,
			success: function(msg){
				jQuery('#td_atividade_'+posicao).html(msg);
			}
		});
		return true; 
	}

	function atualizaComboMacroAtividade( mtmid ){
		jQuery.ajax({
			type: "POST",
			url: "pdeescola.php?modulo=meprincipal/me_questionario_fisico_financeiro&acao=A",
			data: "requisicao=atualizaComboatividade&mtmid="+mtmid,
			success: function(msg){
				jQuery('#atividade_np').html(msg);
			}
		});
		return true; 
	}
	
	function atualizaComboRepresentante( repres ){
		var repres = repres
		jQuery.ajax({
			type: "POST",
			url: "pdeescola.php?modulo=meprincipal/me_questionario_fisico_financeiro&acao=A&tipo_rep="+repres,
			data: "requisicao=atualizaComboRepresentante",
			success: function(msg){
				if(repres == 'seguimento_1'){
					jQuery('#representantepedagogico').html(msg);
				}else if(repres == 'seguimento_2'){
					jQuery('#representanteassuntocomunitario').html(msg);
				}else if(repres == 'seguimento_3'){
					jQuery('#representanteexecucaofinanceira').html(msg);
				}
			}
		});
		return true; 
	}
	
	function carregarNomeCoordenador(){
		jQuery.ajax({
			type: "POST",
			url: "pdeescola.php?modulo=meprincipal/me_questionario_fisico_financeiro&acao=A",
			data: "requisicao=carregarNomeCoordenador&ptrid=",
			async: false,
			success: function(msg){
				alert(msg);
				jQuery('#td_coordernador').html(msg);
			}
		});
	}

	function deletarMacrocampoAtividade(qrpid, mtmid, mtaid, obj){
		jQuery.ajax({
			type : "POST",
			url : "pdeescola.php?modulo=meprincipal/me_questionario_fisico_financeiro&acao=A",
			data : "requisicao=deletarMacrocampoAtividade&qrpid="+qrpid+"&mtmid="+mtmid+"&mtaid="+mtaid,
			success: function(msg){
				var status = jQuery.trim(msg);		
				if(status != "ok"){
					alert(msg);
				}else{
					jQuery(obj).parent().parent().remove();
					alert('O Dado foi excluido com sucesso!');
				}
			}
		});
	}
	
</script>

<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td>
			<fieldset style="width: 94%; background: #fff;"  >
				<legend>Questionário</legend>
				<?php
					$qrpid = pegaQrpidMaisEdFinan();
					$habilitado = buscaPermissaoAcesso();
					$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => $habilitado) );
				?>
			</fieldset>
		</td>
	</tr>
</table>

