<?php
$_SESSION['par']['tipo'] = $_REQUEST['tipo'] ? $_REQUEST['tipo'] : false;
$campo = $_SESSION['par']['muncod'] ? 'muncodpar' 				 : 'estufpar';

if( $_SESSION['par']['estuf'] == 'DF' ){
	$valor = $_SESSION['par']['estuf'] ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
} else {
	$valor = $_SESSION['par']['muncod'] ? $_SESSION['par']['muncod'] : $_SESSION['par']['estuf'];
}

if( $_REQUEST['arqidProrrogacao'] ){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec(NULL, NULL, "obras");
	$arquivo = $file->getDownloadArquivo($_REQUEST['arqidProrrogacao']);
	//echo"<script>window.location.href = 'par.php?modulo=principal/termoAnalise&acao=A';</script>";
	exit;
}

if($_REQUEST['download'] == 'manual'){
	
	$caminho = APPRAIZ."www/par/Orientacoes_para_preenchimento_dos_dados_SIMEC{$_REQUEST['ano']}.pdf";
	if ( !is_file( $caminho ) ) {
        echo"<script>
            	alert('Arquivo não encontrado na pasta.');
            	window.location.href = 'par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&programa={$_REQUEST['programa']}';
            </script>";
    	exit();
	} 
    $filename = "Orientacoes_para_preenchimento_dos_dados_SIMEC{$_REQUEST['ano']}.pdf";
    header( 'Content-type: pdf' );
    header( 'Content-Disposition: attachment; filename='.$filename);
    readfile( $caminho );
    echo"<script>window.location.href = 'par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&programa={$_REQUEST['programa']}';</script>";	
} 


if($_REQUEST['download'] == 'planilha'){
	ob_clean();
	$caminho = APPRAIZ."www/par/planilha_de_calculo{$_REQUEST['ano']}.xlsx";
	$filename = "planilha_de_calculo{$_REQUEST['ano']}.xlsx";
	header( 'Content-type: xlsx' );
	header( 'Content-Disposition: attachment; filename='.$filename);
	readfile( $caminho );
	exit;
	//echo"<script>window.location.href = 'par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&programa={$_REQUEST['programa']}';</script>";
}

// Carga preprioridade 
/*
if($_SESSION['usucpf'] == '' && $_POST['requisicao'] == 'carga'){
	if($_POST['tipo'] == "semCobertura"){
		$sinal = "<>";
	} else {
		$sinal = "=";
	}
	
	$sql = "select m.muncod
		from territorios.municipio m
	LEFT JOIN par.instrumentounidade iu ON iu.mun_estuf = m.estuf AND iu.muncod = m.muncod AND iu.itrid = 2
	order by m.mundescricao";
	$arMunicipio = $db->carregar($sql);
	foreach($arMunicipio as $municipio){
			$sql = "SELECT 
						pre.preid, 
					  	pre.docid, 
					  	pre.presistema,
					  	pre.estuf,
					  	pre.muncod,
					  	pre.predtinclusao,
					  	pre.predescricao,
					  	pre.preano,
					  	pre.pretipofundacao,
					  	esd.esddsc,
						pto.ptodescricao,
						pre.preprioridade
					FROM obras.preobra pre
					INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
					LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
					LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
					WHERE pre.muncod = '{$municipio['muncod']}' 
					AND pre.presistema = '23'
					AND pre.ptoid $sinal ".OBRA_QUADRA_COBERTURA."
					AND pre.prestatus = 'A'
					AND pre.tooid =".ORIGEM_OBRA_PAC2."";
			$arDados = $db->carregar($sql);
			$arDados = ($arDados) ? $arDados : array();
			$count = 1;
			foreach($arDados as $dados){
				$sql = "UPDATE obras.preobra SET preprioridade = $count where preid = {$dados['preid']}";
				//echo $sql."<br>";
				$count++;
				$db->executar($sql);
			}
	}
	$status = $db->commit();
	echo $status;
	die;
}
*/
if($_POST['requisicao'] == 'mudaPosicao'){
	extract($_POST);
	
	if($tipo == "semCobertura"){
		$sinal = "<>";
	} else {
		$sinal = "=";
	}
	
	$count = $db->pegaUm("SELECT count(1) FROM obras.preobra WHERE muncod = '{$_SESSION['par']['muncod']}' AND presistema = '23' AND prestatus = 'A' AND ptoid $sinal ".OBRA_QUADRA_COBERTURA." ");
	if($preid1){
		$ordem1 = $db->pegaUm("SELECT preprioridade FROM obras.preobra WHERE preid = {$preid1}");
	}
	//ver("SELECT preprioridade FROM obras.preobra WHERE preid = {$preid1}");
	
	if($posicao == 'cima'){
		$preprioridade = ($ordem1-1);
	} elseif($posicao == 'baixo'){
		$preprioridade = ($ordem1+1);
	}
	if(($ordem1 == 1 && $posicao == 'cima') || ($ordem1 == $count && $posicao == 'baixo')) {
		die();
	}
		
	$preid2 = $db->pegaUm("SELECT preid FROM obras.preobra WHERE preprioridade = ".$preprioridade." and muncod = '{$_SESSION['par']['muncod']}' AND presistema = '23' AND prestatus = 'A' AND ptoid $sinal ".OBRA_QUADRA_COBERTURA." ");
	
	//ver("SELECT preid FROM obras.preobra WHERE preprioridade = ".$preprioridade." and muncod = '{$_SESSION['par']['muncod']}' AND presistema = '23' AND prestatus = 'A' AND ptoid $sinal ".OBRA_QUADRA_COBERTURA." ",d);
	if($preid2){
		$ordem2 = $db->pegaUm("SELECT preprioridade FROM obras.preobra WHERE preid = {$preid2}");
	}
	
	if($ordem1 && $ordem2){
		$db->executar("UPDATE obras.preobra SET preprioridade = '$ordem1' WHERE preid = {$preid2}");
		$db->executar("UPDATE obras.preobra SET preprioridade = '$ordem2' WHERE preid = {$preid1}");
		if($db->commit()){
			echo 'sucesso';
		} else {
			echo 'erro';
		}
	}
	exit;
}

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_GET['download'] == 'S'){
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    echo"<script>window.location.href = 'par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A';</script>";
    exit;
}

$obPreObraControle 	  	= new PreObraControle();
$obEntidadeParControle 	= new EntidadeParControle();

$muncod = $_SESSION['par']['muncod']; 
$nrAdesao = $obPreObraControle->verificaPrazoExpiraAdesao($muncod);
$booQtdObrasEstado = true;
if($_REQUEST['tipo']){
	$booQtdObrasEstado = $obPreObraControle->verificaQtdObrasEstado(Array('estuf'=>$_SESSION['par']['estuf'],'tipo'=>$_REQUEST['tipo']));
}
$boAdesao = $nrAdesao > 0 ? true : false;

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

$podeProrrogar = false;
if( in_array( PAR_PERFIL_PREFEITO,$perfil ) ||
	in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$perfil ) ||
	in_array( PAR_PERFIL_ADMINISTRADOR,$perfil ) ||
	in_array( PAR_PERFIL_SUPER_USUARIO,$perfil )
){
	$podeProrrogar = true;
}

$boData = false;
if( verificaGrupoMunicipioMUNCOD( $muncod ) ){
	if(date('YmdHis') <= DATA_EXPIRA_PROINFANCIA_PENDENCIAS){
		$boData = true;
	}
}else{
	if(date('YmdHis') <= DATA_EXPIRA_PROINFANCIA){
		$boData = true;
	}
}

if($_REQUEST['programa']){
	$cotaproinfancia = verificarQtdObraProinfancia(); 
	$qtdtdpreobra = verificarQtdPreObraProinfancia();
	
	if($cotaproinfancia > $qtdtdpreobra){
		$hab_prog_proinf = true;
	} else {
		$hab_prog_proinf = false;
	}
	
}
// ver($_SESSION['par']);

//if(!$boAdesao && !$boData){
//	
//	if(!in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) && 
//	   !in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) &&
//	   !in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) &&
//	   !in_array(PAR_PERFIL_COORDENADOR_TECNICO, $perfil)){
//	
//		echo "<script>
//				alert('Encerrado prazo para inscrições em 29/10/2010.');
//				document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
//			  </script>";
//	}
//}

$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

include_once APPRAIZ . 'includes/workflow.php';
//include_once APPRAIZ . 'includes/cabecalho.inc';

unset($_SESSION['par']['preid']);

if($_REQUEST['programa']){
	$_SESSION['proinfancia2014'] = true;
	$preano = '2014';
	$hab_prog_proinf = false;
}

$stUF 		  = $obPreObraControle->recuperarUF($muncod);
$busca = Array( 'campo' => $campo , 'valor' => $valor, 'tipo' => $_REQUEST['tipo'] );
$arPreObrasSemCobertura	  = $obPreObraControle->recuperarPreObraPorMuncodSemQuadraCobertura($busca, SIS_OBRAS, $preano);
$arPreObrasSemCobertura   = $arPreObrasSemCobertura ? $arPreObrasSemCobertura : array();

$arPreObrasComCobertura	  = $obPreObraControle->recuperarPreObraPorMuncodComQuadraCobertura($busca, SIS_OBRAS);
$arPreObrasComCobertura   = $arPreObrasComCobertura ? $arPreObrasComCobertura : array();

$arPreObrasCobertura	  = $obPreObraControle->recuperarPreObraPorMuncodCobertura($busca, SIS_OBRAS);
$arPreObrasCobertura   	  = $arPreObrasCobertura ? $arPreObrasCobertura : array();
//ver($arPreObrasCobertura);

if($_GET['excluir']){
	
	if($obPreObraControle->excluirPreObra($_GET['excluir']) && $escrita ){
		echo "<script>
				alert('Operação realizada com sucesso.');
				document.location.href = 'par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A';
			  </script>";
	}
	exit();
}


if ($_REQUEST['ajaxMontarTabela']) {
	carregarTabelaProInfancia($arPreObrasSemCobertura, $obPreObraControle, $hab_prog_proinf, $escrita);
	die;
}

include_once APPRAIZ . 'includes/cabecalho.inc';

$descricao = $obEntidadeParControle->recuperaDescricaoEntidadePar($_SESSION['par']['muncod'], null);

//$nrQtdObras   = count($arPreObrasSemCobertura);
//$nrQtdMaxima  = $obPreObraControle->recuperarQuantidadePac($_SESSION['par']['muncod']);

//if(!$nrQtdMaxima){
//	echo "<script>
//			alert('Este município não tem direito ao programa Pró infância');
//			document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
//		  </script>";	
//}
?>
<br />
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
<script type="text/javascript" src="../includes/funcoes.js" ></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>		
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript">
	jQuery.noConflict();
</script>
<script type="text/javascript"><!--
jQuery(document).ready(function(){	
	
	/*jQuery('.removeItens').live('click',function(){
		var filhos = jQuery('#itensComposicaoSemQuadraCobertura').children().length;		
		if (filhos > 1) {
			jQuery(this).parent().parent().remove();
		}
		return false;
	});*/
	
	jQuery('.voltar').live('click',function(){

		document.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';	
		
	});

	jQuery('.alterar').live('click',function(){

		return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+this.id, 
						   'ProInfancia', 
						   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
		
	});

	jQuery('.prorrogar').live('click',function(){

		return window.open('par.php?modulo=principal/popupProrrogacaoPAC&acao=A&preid='+this.id, 
						   'Prorrogação', 
						   "height=400,width=600,scrollbars=yes,top=50,left=200" ).focus();		
		
	});

	jQuery('.consultaprorrogacao').live('click',function(){

		return window.open('par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&arqidProrrogacao='+this.id, 
						   'Consulta prorrogação', 
						   "height=200,width=200,scrollbars=yes,top=50,left=200" ).focus();		
		
	});

	jQuery('.adicionar').live('click',function(){

//		var totalObras 		= <?php echo $nrQtdObras; ?>;
//		var totalPermitido 	= <?php echo $nrQtdMaxima; ?>;
		var escrita = <?=$escrita; ?>;
		var anoAtual = <?php echo date('Y'); ?>;
		var boData = <?=($boData) ? "true" : "false" ?>;		
//		if(totalObras < totalPermitido){
//		if( escrita &&  boData){
			return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&ano='+anoAtual+'&prog=proinf', 
					   'ProInfancia',
					   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
//		}else{
//			alert('Encerrado prazo para inscrições em 29/10/2010.');
//		}
//		}else{
			
//			alert('Número máximo de obras permitidos.');
//		}		
	});

	jQuery('.excluir').click(function(){

		if(!confirm("Será excluído todos os dados desta obra, deseja continuar?")){
			return false;
		}
		
		document.location.href = 'par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&excluir='+this.id;
	});

	jQuery('.anexos').live('click',function(){
		var arDados = this.id.split("_");		
		return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='+arDados[0]+'&preid='+arDados[1]+'', 
				   'ProInfancia',
				   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
		
	});
	
	jQuery('.cimaSemCobertura').live('click',function(){
		// Pega a quantidade
		var qtdTr = jQuery('#itensComposicaoSemQuadraCobertura').children().length;
		// Pega a tr atual
		var trAtual = jQuery(this).closest('tr');
		// Pegamos o preid do atual
		var preidAtual = trAtual.attr('id');
		// Pegamos a tr anterior
		var trAnterior = trAtual.prev();

		var cor1 = '#f7f7f7';
		var cor2 = trAnterior.attr('bgcolor');
		if(cor2 == '#f7f7f7'){
			cor1 = '#ffffff';
		}
		
		// Pegamos o preid anterior
		var preidAnterior = trAnterior.attr('id');

		if(preidAtual && preidAnterior){
			var param = new Array();
			param.push({name : 'requisicao', value : 'mudaPosicao'},
					   {name : 'posicao', value : 'cima'},
					   {name : 'tipo', value : 'semCobertura'},
					   {name : 'preid1', value : preidAtual}
					  );
			  
			jQuery.ajax({
				type	 : "POST",
				url		 : "par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A",
				data	 : param,
				async    : false,
				//dataType : 'json',
				success	 : function(data){
								//alert(data)
								if(data == 'sucesso'){
									trAtual.prev().before(trAtual);

									var numeracaoAtual = trAtual.find('span').text();
									trAtual.mouseout(function(){
										jQuery(this).attr('bgcolor',cor2);
									});
									/*
									* Tratamento quando a tr atual ta em primeiro ou em penultimo registro
									*/
									// se a tr atual fica na primeira posicao. entao desabilita a seta de cima
									if(trAtual.index() == 0){
										trAtual.find('img[class=cimaSemCobertura]').attr('src','../imagens/seta_cimad.gif');
										trAtual.find('img[class=cimaSemCobertura]').css('cursor','');
									}
									// se a tr atual fica na penultima posicao. entao habilita a seta de baixo
									if(trAtual.index() == (qtdTr-2)){
										trAtual.find('img[class=baixoSemCobertura]').attr('src','../imagens/seta_baixo.gif');
										trAtual.find('img[class=baixoSemCobertura]').css('cursor','pointer');
									}
									
									/*
									* Fim do tratamento
									*/
									
									/*
									* Tratamento quando a tr proxima fica em segundo ou em ultimo registro
									*/
									var trProxima = trAtual.next();

									trAtual.find('span').html('<strong>'+trProxima.find('span').text()+'</strong>');
									trAtual.attr('bgcolor',cor2);
									// se a tr proxima fica na segunda posicao. entao habilita a seta de cima
									if(trProxima.index() == 1){
										trProxima.find('img[class=cimaSemCobertura]').attr('src','../imagens/seta_cima.gif');
										trProxima.find('img[class=cimaSemCobertura]').css('cursor','pointer');
									}
									// se a tr proxima fica na ultima posicao. entao desabilita a seta de baixo
									if(trProxima.index() == (qtdTr-1)){
										trProxima.find('img[class=baixoSemCobertura]').attr('src','../imagens/seta_baixod.gif');
										trProxima.find('img[class=baixoSemCobertura]').css('cursor','');
									}

									trProxima.find('span').html('<strong>'+numeracaoAtual+'</strong>');
									trProxima.attr('bgcolor',cor1);
									trProxima.mouseout(function(){
										jQuery(this).attr('bgcolor',cor1);
									});
									/*
									* Fim do tratamento
									*/

								} else if(data == 'erro') {
									alert('Ocorreu um erro ao salvar.');
								}
						  }
				 });
			
		}
		
	});

	jQuery('.baixoSemCobertura').live('click',function(){
		// Pega a quantidade
		var qtdTr = jQuery('#itensComposicaoSemQuadraCobertura').children().length;
		// Pega a tr atual
		var trAtual = jQuery(this).closest('tr');
		// Pegamos o preid do atual
		var preidAtual = trAtual.attr('id');
		// Pegamos a tr anterior
		var trProxima = trAtual.next();
		
		var cor1 = '#f7f7f7';
		var cor2 = trProxima.attr('bgcolor');
		if(cor2 == '#f7f7f7'){
			cor1 = '#ffffff';
		}
				
		// Pegamos o preid anterior
		var preidProxima = trProxima.attr('id');

		if(preidAtual && preidProxima){
			var param = new Array();
			param.push({name : 'requisicao', value : 'mudaPosicao'},
					   {name : 'posicao', value : 'baixo'},
					   {name : 'tipo', value : 'semCobertura'},
					   {name : 'preid1', value : preidAtual}
					  );
			  
			jQuery.ajax({
				type	 : "POST",
				url		 : "par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A",
				data	 : param,
				async    : false,
				//dataType : 'json',
				success	 : function(data){
								if(data == 'sucesso'){
									trAtual.next().after(trAtual);

									var numeracaoAtual = trAtual.find('span').text();
									trAtual.mouseout(function(){
										jQuery(this).attr('bgcolor',cor2);
									});
									
									/*
									* Tratamento quando a tr atual ta em primeiro ou em ultimo registro
									*/
									// se a tr atual fica na segunda posicao. entao habilita a seta de cima e seta baixo
									if(trAtual.index() == 1){
										trAtual.find('img[class=baixoSemCobertura]').attr('src','../imagens/seta_baixo.gif');
										trAtual.find('img[class=baixoSemCobertura]').css('cursor','pointer');
										
										trAtual.find('img[class=cimaSemCobertura]').attr('src','../imagens/seta_cima.gif');
										trAtual.find('img[class=cimaSemCobertura]').css('cursor','pointer');
									}
									// se a tr atual fica na ultima posicao. entao desabilita a seta de baixo
									if(trAtual.index() == (qtdTr-1)){
										trAtual.find('img[class=baixoSemCobertura]').attr('src','../imagens/seta_baixod.gif');
										trAtual.find('img[class=baixoSemCobertura]').css('cursor','');
									}
									/*
									* Fim do tratamento do primeiro e o segundo registro
									*/
									
									/*
									* Tratamento quando a tr anterior fica na primeira ou na penultima registro
									*/
									var trAnterior = trAtual.prev();
									trAtual.find('span').html('<strong>'+trAnterior.find('span').text()+'</strong>');
									trAtual.attr('bgcolor',cor2);
									
									// se a tr anterior fica na primeira posicao. entao desabilita a seta de cima
									if(trAnterior.index() == 0){
										trAnterior.find('img[class=cimaSemCobertura]').attr('src','../imagens/seta_cimad.gif');
										trAnterior.find('img[class=cimaSemCobertura]').css('cursor','');
									}
									// se a tr anterior fica na penultima posicao. entao habilita a seta de baixo
									if(trAnterior.index() == (qtdTr-2)){
										trAnterior.find('img[class=baixoSemCobertura]').attr('src','../imagens/seta_baixo.gif');
										trAnterior.find('img[class=baixoSemCobertura]').css('cursor','pointer');
									}

									trAnterior.find('span').html('<strong>'+numeracaoAtual+'</strong>');
									trAnterior.attr('bgcolor',cor1);
									trAnterior.mouseout(function(){
										jQuery(this).attr('bgcolor',cor1);
									});

								} else if(data == 'erro') {
									alert('Ocorreu um erro ao salvar.');
								}
						  }
				 });
		}
	});

	jQuery('.cimaComCobertura').live('click',function(){
		// Pega a quantidade
		var qtdTr = jQuery('#itensComposicaoComQuadraCobertura').children().length;
		// Pega a tr atual
		var trAtual = jQuery(this).closest('tr');
		// Pegamos o preid do atual
		var preidAtual = trAtual.attr('id');
		// Pegamos a tr anterior
		var trAnterior = trAtual.prev();

		var cor1 = '#f7f7f7';
		var cor2 = trAnterior.attr('bgcolor');
		if(cor2 == '#f7f7f7'){
			cor1 = '#ffffff';
		}
		
		// Pegamos o preid anterior
		var preidAnterior = trAnterior.attr('id');

		if(preidAtual && preidAnterior){
			var param = new Array();
			param.push({name : 'requisicao', value : 'mudaPosicao'},
					   {name : 'posicao', value : 'cima'},
					   {name : 'tipo', value : 'comCobertura'},
					   {name : 'preid1', value : preidAtual}
					  );
			  
			jQuery.ajax({
				type	 : "POST",
				url		 : "par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A",
				data	 : param,
				async    : false,
				//dataType : 'json',
				success	 : function(data){
								if(data == 'sucesso'){
									trAtual.prev().before(trAtual);

									var numeracaoAtual = trAtual.find('span').text();
									trAtual.mouseout(function(){
										jQuery(this).attr('bgcolor',cor2);
									});
									/*
									* Tratamento quando a tr atual ta em primeiro ou em penultimo registro
									*/
									// se a tr atual fica na primeira posicao. entao desabilita a seta de cima
									if(trAtual.index() == 0){
										trAtual.find('img[class=cimaComCobertura]').attr('src','../imagens/seta_cimad.gif');
										trAtual.find('img[class=cimaComCobertura]').css('cursor','');
									}
									// se a tr atual fica na penultima posicao. entao habilita a seta de baixo
									if(trAtual.index() == (qtdTr-2)){
										trAtual.find('img[class=baixoComCobertura]').attr('src','../imagens/seta_baixo.gif');
										trAtual.find('img[class=baixoComCobertura]').css('cursor','pointer');
									}
									
									/*
									* Fim do tratamento
									*/
									
									/*
									* Tratamento quando a tr proxima fica em segundo ou em ultimo registro
									*/
									var trProxima = trAtual.next();

									trAtual.find('span').html('<strong>'+trProxima.find('span').text()+'</strong>');
									trAtual.attr('bgcolor',cor2);
									// se a tr proxima fica na segunda posicao. entao habilita a seta de cima
									if(trProxima.index() == 1){
										trProxima.find('img[class=cimaComCobertura]').attr('src','../imagens/seta_cima.gif');
										trProxima.find('img[class=cimaComCobertura]').css('cursor','pointer');
									}
									// se a tr proxima fica na ultima posicao. entao desabilita a seta de baixo
									if(trProxima.index() == (qtdTr-1)){
										trProxima.find('img[class=baixoComCobertura]').attr('src','../imagens/seta_baixod.gif');
										trProxima.find('img[class=baixoComCobertura]').css('cursor','');
									}

									trProxima.find('span').html('<strong>'+numeracaoAtual+'</strong>');
									trProxima.attr('bgcolor',cor1);
									trProxima.mouseout(function(){
										jQuery(this).attr('bgcolor',cor1);
									});
									/*
									* Fim do tratamento
									*/

								} else if(data == 'erro') {
									alert('Ocorreu um erro ao salvar.');
								}
						  }
				 });
			
		}
		
	});

	jQuery('.baixoComCobertura').live('click',function(){
		// Pega a quantidade
		var qtdTr = jQuery('#itensComposicaoComQuadraCobertura').children().length;
		// Pega a tr atual
		var trAtual = jQuery(this).closest('tr');
		// Pegamos o preid do atual
		var preidAtual = trAtual.attr('id');
		// Pegamos a tr anterior
		var trProxima = trAtual.next();
		
		var cor1 = '#f7f7f7';
		var cor2 = trProxima.attr('bgcolor');
		if(cor2 == '#f7f7f7'){
			cor1 = '#ffffff';
		}
				
		// Pegamos o preid anterior
		var preidProxima = trProxima.attr('id');

		if(preidAtual && preidProxima){
			var param = new Array();
			param.push({name : 'requisicao', value : 'mudaPosicao'},
					   {name : 'posicao', value : 'baixo'},
					   {name : 'tipo', value : 'comCobertura'},
					   {name : 'preid1', value : preidAtual}
					  );
			  
			jQuery.ajax({
				type	 : "POST",
				url		 : "par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A",
				data	 : param,
				async    : false,
				//dataType : 'json',
				success	 : function(data){
								if(data == 'sucesso'){
									trAtual.next().after(trAtual);

									var numeracaoAtual = trAtual.find('span').text();
									trAtual.mouseout(function(){
										jQuery(this).attr('bgcolor',cor2);
									});
									
									/*
									* Tratamento quando a tr atual ta em primeiro ou em ultimo registro
									*/
									// se a tr atual fica na segunda posicao. entao habilita a seta de cima e seta baixo
									if(trAtual.index() == 1){
										trAtual.find('img[class=baixoComCobertura]').attr('src','../imagens/seta_baixo.gif');
										trAtual.find('img[class=baixoComCobertura]').css('cursor','pointer');
										
										trAtual.find('img[class=cimaComCobertura]').attr('src','../imagens/seta_cima.gif');
										trAtual.find('img[class=cimaComCobertura]').css('cursor','pointer');
									}
									// se a tr atual fica na ultima posicao. entao desabilita a seta de baixo
									if(trAtual.index() == (qtdTr-1)){
										trAtual.find('img[class=baixoComCobertura]').attr('src','../imagens/seta_baixod.gif');
										trAtual.find('img[class=baixoComCobertura]').css('cursor','');
									}
									/*
									* Fim do tratamento do primeiro e o segundo registro
									*/
									
									/*
									* Tratamento quando a tr anterior fica na primeira ou na penultima registro
									*/
									var trAnterior = trAtual.prev();
									trAtual.find('span').html('<strong>'+trAnterior.find('span').text()+'</strong>');
									trAtual.attr('bgcolor',cor2);
									
									// se a tr anterior fica na primeira posicao. entao desabilita a seta de cima
									if(trAnterior.index() == 0){
										trAnterior.find('img[class=cimaComCobertura]').attr('src','../imagens/seta_cimad.gif');
										trAnterior.find('img[class=cimaComCobertura]').css('cursor','');
									}
									// se a tr anterior fica na penultima posicao. entao habilita a seta de baixo
									if(trAnterior.index() == (qtdTr-2)){
										trAnterior.find('img[class=baixoComCobertura]').attr('src','../imagens/seta_baixo.gif');
										trAnterior.find('img[class=baixoComCobertura]').css('cursor','pointer');
									}

									trAnterior.find('span').html('<strong>'+numeracaoAtual+'</strong>');
									trAnterior.attr('bgcolor',cor1);
									trAnterior.mouseout(function(){
										jQuery(this).attr('bgcolor',cor1);
									});

								} else if(data == 'erro') {
									alert('Ocorreu um erro ao salvar.');
								}
						  }
				 });
		}
	});


	jQuery('#semCobertura').live('click',function(){
		
			var param = new Array();
			param.push({name : 'requisicao', value : 'carga'},
					   {name : 'tipo', value : 'semCobertura'}
					  );
			  
			jQuery.ajax({
				type	 : "POST",
				url		 : "par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A",
				data	 : param,
				async    : false,
				//dataType : 'json',
				success	 : function(data){
							if(data == 1){
								alert("carga concluida");
							}					
						}
				 });
		
	});

	jQuery('#comCobertura').live('click',function(){
		
		var param = new Array();
		param.push({name : 'requisicao', value : 'carga'},
				   {name : 'tipo', value : 'comCobertura'}
				  );
		  
		jQuery.ajax({
			type	 : "POST",
			url		 : "par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A",
			data	 : param,
			async    : false,
			//dataType : 'json',
			success	 : function(data){
						if(data == 1){
							alert("carga concluida");
						}					
					}
			 });
	
});

});

function focamensagem(){
	document.getElementById('msg').focus();
}

--></script>
<!--  <div style="color: red; font-size: 14px; font-weight: bold; text-align: center; padding-bottom: 5px;" > Devido a inoperância do sistema no período de 20h25min as 20h45min do dia 30/11/2011, prorrogaremos o prazo de envio até a 01h00min do dia 01/12/2011. </div> -->
<?php if( $_REQUEST['programa'] != 'proinfancia2014' ){ ?>
<table class="tabela" style="width:95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td><b>Manual de preenchimento das obras do PAC</b></td>
	</tr>
	<tr>
		<td align="left" valign="top" width="90%">
			
			<table class="tabela" style="width:100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<thead>			
					<tr bgcolor="#e9e9e9" align="left">
						<td width="80px" align="left">
							<a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/programas/proinfancia/proInfancia&acao=A&programa=proinfancia2014&download=manual'"><b><img src="../imagens/pdf.gif" width="18px" border="0"> Manual de Preenchimento</b></a>
						</td>
					</tr>
				</thead>
			</table>
		</td>
	</tr>
</table>
<br>
<? }elseif( $_REQUEST['programa'] == 'proinfancia2014' ){ ?>
<table class="tabela" style="width:95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="30%" colspan="2" ><b>Manual de preenchimento das obras do PAC</b></td>
		<!--  td style="color: red; font-size: 14px; text-align: left;"><b>Prazo para o envio de projetos do Proinfância 2014 é até 30 de novembro de 2013</b></td> -->
	</tr>
	<tr>
		<td align="left" valign="top" width="90%" colspan="2">
			
			<table class="tabela" style="width:100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<thead>			
					<tr bgcolor="#e9e9e9" align="left">
						<td width="80px" align="left">
							<a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/programas/proinfancia/proInfancia&acao=A&programa=proinfancia2014&download=manual&ano=2014'">
								<b><img src="../imagens/pdf.gif" width="18px" border="0"> Manual de Preenchimento</b>
							</a>
							<a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/programas/proinfancia/proInfancia&acao=A&programa=proinfancia2014&download=planilha&ano=2014'">
								<b><img src="../imagens/excel.gif" width="18px" border="0"> Planilha de Cálculo</b>
							</a>
						</td>
					</tr>
				</thead>
			</table>
		</td>
	</tr>
</table>
<br>
<?php 
}
if( date('Y-m-d') <= '2013-05-30' ){ ?>
<table class="tabela" style="width:95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="color: red; font-size: 14px; text-align: center;"><b>Projetos de Quadras, Coberturas e Creches deverão ser cadastrados até 31 de Maio de 2013.</b></td>
	</tr>
</table>
<br>
<?php }

switch ($_REQUEST['tipo']){
	case 'Q':
		$titulo = 'Quadras Estaduais - '.$db->pegaUm('SELECT estdescricao FROM territorios.estado WHERE estuf = \''.$_SESSION['par']['estuf'].'\'');
		break;
	case 'C':
		$titulo = 'Cobertura de Quadras Estaduais - '.$db->pegaUm('SELECT estdescricao FROM territorios.estado WHERE estuf = \''.$_SESSION['par']['estuf'].'\'');
		break;
	default:
		$titulo = 'Pró Infância';
		break;
}

monta_titulo( $titulo, $descricao  );	
if(!$_REQUEST['tipo']||$_SESSION['par']['estuf']=='DF'){?>
<?php 
/*if($_SESSION['usucpf'] == ''){
	?>
	
	<input type="button" id="semCobertura" value="semCobertura">
	<input type="button" id="comCobertura" value="comCobertura">
	
	<?php
}*/
?>
<div id="div-tabela">
	<?php carregarTabelaProInfancia($arPreObrasSemCobertura, $obPreObraControle, $hab_prog_proinf, $escrita, $podeProrrogar); ?>
</div>		
<?php } ?>		
<br />
<?php if(empty($_REQUEST['programa'])){ 
if(count($arPreObrasComCobertura)>0){
monta_titulo( 'Quadras', $descricao  );?>
<table class="tabela" style="width:95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align="left" valign="top" width="90%">
			<table class="tabela" style="width:100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<thead>			
					<tr bgcolor="#e9e9e9" align="center">
						<td width="80px" align="center"><b>Prioridade</b>&nbsp<img src="../imagens/ajuda.png" onclick="focamensagem()" width="13px" style="cursor:pointer" /></td>
						<td width="55px" align="center"><b>Ação</b></td>
						<td align="left"><b>Nome da obra</b></td>
						<td align="left"><b>Tipo de obra</b></td>
						<td align="left"><b>Situação</b></td>
						<td align="left"><b>Termo</b></td>
						<td align="left"><b>Empenho processo</b></td>
						<td align="left"><b>Pagamento</b></td>
						<td align="left"><b>Fim da vigência</b></td>
						<td align="left"><b>Prorrogar (dias)</b></td>
						<td align="left"><b>Processo</b></td>
					</tr>
				</thead>
				<tbody id="itensComposicaoComQuadraCobertura">
					<?php $x=0; ?>
					<?php if(count($arPreObrasComCobertura)): ?>
						<?php foreach($arPreObrasComCobertura as $preobra): ?>
							<?php
							$docid = prePegarDocid($preobra['preid']);
							$esdid = prePegarEstadoAtual($docid); 				
							$cor = ($x % 2) ? "#F7F7F7" : "white";
							
							$seta_cima = '../imagens/seta_cima.gif';
							$styleCima = 'style="cursor:pointer;"';
							if($x == 0){
								$styleCima = '';
								$seta_cima = '../imagens/seta_cimad.gif';
							}
							
							$seta_baixo = '../imagens/seta_baixo.gif';
							$styleBaixo = 'style="cursor:pointer;"';
							if(($x + 1) == count($arPreObrasComCobertura)){
								$styleBaixo = '';
								$seta_baixo = '../imagens/seta_baixod.gif';
							}
							
							?>					
							<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';" id="<?php echo $preobra['preid'];?>">
								<td >
									<span><strong><?php echo str_pad($preobra['preprioridade'], 3, "0", STR_PAD_LEFT); ?></strong></span>&nbsp;&nbsp;&nbsp;
									<img class="baixoComCobertura" src="<?php echo $seta_baixo; ?>" <?php echo $styleBaixo ?> title="Diminuir Prioridade" />&nbsp;
									<img class="cimaComCobertura" src="<?php echo $seta_cima; ?>" <?php echo $styleCima ?> title="Aumentar Prioridade" />
								</td>				
								<td style="padding-left:15px;">
									<?php if($esdid == WF_TIPO_EM_CADASTRAMENTO): ?>
<!--										<img class="excluir" src="../imagens/excluir.gif" id="<?php echo $preobra['preid'] ?>" style="cursor:pointer;" />&nbsp;&nbsp;-->
										<img src="../imagens/alterar.gif" class="alterar" style="cursor:pointer" id="<?php echo $preobra['preid'] ?>" />
									<?php else: ?>
										<!-- img src="../imagens/excluir_01.gif" />&nbsp;&nbsp; -->
										<img src="../imagens/alterar.gif" class="alterar" style="cursor:pointer" id="<?php echo $preobra['preid'] ?>" />
									<?php endif; ?>
									<?php $data_primeiro_pagamento = $db->pegaUm("SELECT 
																					MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento
																				FROM 
																					par.pagamentoobra po 
																				INNER JOIN par.pagamento pag ON pag.pagid = po.pagid and pagstatus = 'A'
																				WHERE 
																					po.preid = ".$preobra['preid']);
									
									$sql = "SELECT
												doc.esdid
											FROM
												obras.preobra pre
											INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
											LEFT  JOIN obras2.obras obr 
												INNER JOIN workflow.documento doc ON doc.docid = obr.docid
											ON obr.preid = pre.preid
											WHERE
												pre.preid = {$preobra['preid']}";
									
									$verificaSituacao = $db->pegaUm( $sql );
																		
									/*$arrEstadoObra = Array(
															OBR_ESDID_ETAPA_CONCLUIDA, 
															OBR_ESDID_EM_ELABORACAO_DE_PROJETOS, 
															OBR_ESDID_EM_EXECUCAO, 
															OBR_ESDID_EM_LICITACAO, 
															OBR_ESDID_EM_REFORMULACAO, 
															OBR_ESDID_PARALISADA)*/;
										/*
										 * A pedido do Francisco dia 10/12/2013
										*/
										$arrEstadoObra = Array(); // Array(OBR_ESDID_CONCLUIDA);
									
//									if( $podeProrrogar && $data_primeiro_pagamento && ($verificaSituacao == 12 || $verificaSituacao == 99 || $verificaSituacao == 1 || $verificaSituacao == 5 || $verificaSituacao == 9 || $verificaSituacao == 2 ) ):
									$sqlPrazo= "SELECT
													coalesce(MAX(popdataprazoaprovado)::date, (MIN(pag.pagdatapagamentosiafi) + 720)::date ) - now()::date as prazo
												FROM
													par.pagamentoobra po
												INNER JOIN par.pagamento 			pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
												LEFT  JOIN obras.preobraprorrogacao pop ON pop.preid = po.preid AND popstatus = 'A'
												WHERE
													po.preid = {$preobra['preid']}";
									
									
									// $venceuVigenciaDoTermo =  verificaVigenciaVenciada($preobra['preid']);
									// Demanda 950 - A pedido do Tiago Randuz
									$venceuVigenciaDoTermo = true;
									$prazo = $db->pegaUm($sqlPrazo);
									
									if( $podeProrrogar && $data_primeiro_pagamento && !in_array($verificaSituacao,$arrEstadoObra) && $venceuVigenciaDoTermo ){
										if( $esdid != WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO && $prazo < 91 ){ ?>
										<br/><button type="button" class="prorrogar" id="<?php echo $preobra['preid'] ?>" style="font-size: 9px;">
											<?php echo "Solicitar Prorrogação<br/>da Vigência" ?>	
										</button>	
									<?php }
									} else {
										if( in_array($verificaSituacao, array(OBR_ESDID_CONCLUIDA)) && (in_array( PAR_PERFIL_ADM_OBRAS,$perfil ) || in_array( PAR_PERFIL_SUPER_USUARIO,$perfil ) ) && $esdid != WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO ){
											$sql = "SELECT 
													    sum(po.pobpercentualpag)
													FROM 
													    par.pagamentoobra po 
													INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
													WHERE 
													    po.preid = {$preobra['preid']}
													    and pag.pagsituacaopagamento not ilike '%CANCELADO%'";
											$percent = $db->pegaUm($sql);
											
											if( $percent < 100 ){?>
												<br/><button type="button" class="prorrogar" id="<?php echo $preobra['preid'] ?>" style="font-size: 9px;"><?php echo "Solicitar Prorrogação<br/>da Vigência" ?></button><?
											}
										}
											?>
									<?} ?>
								</td>				
								<td>						
									<a class="alterar" id="<?php echo $preobra['preid'] ?>"><?php echo $preobra['predescricao'] ?></a>
									<?php if($obPreObraControle->verificaFotosObras($preobra['preid'])): ?>
										<a href="javascript:void(0)">
											<img class="anexos" id="foto_<?php echo $preobra['preid'] ?>" align="absmiddle" border="0" src="../imagens/cam_foto.gif" title="Fotos cadastradas" />
										</a>							
									<?php endif; ?>
									<?php if($obPreObraControle->verificaAnexoObras($preobra['preid'])): ?>
										<a href="javascript:void(0)">
											<img class="anexos" id="documento_<?php echo $preobra['preid'] ?>" align="absmiddle" border="0" src="../imagens/clipe.gif" title="Documentos anexos" />
										</a>							
									<?php endif; ?>													
								</td>
								<td>
									<?php echo $preobra['ptodescricao'] ?>							
								</td>
								<?php //if(in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)){?>
								<td>
									<?php 
									/* Retirado a pedido do Tiago Radunz no dia 22/07/2014 */
// 								  	if( $preobra['esdid'] == WF_TIPO_EM_CADASTRAMENTO || 
// 								  		$preobra['esdid'] == WF_TIPO_OBRA_APROVADA || 
// 								  		$preobra['esdid'] == WF_TIPO_OBRA_ARQUIVADA ||
// 								  		$preobra['esdid'] == WF_TIPO_EM_REFORMULACAO || 
// 								  		$preobra['esdid'] == WF_TIPO_EM_CORRECAO){
// 								  		echo $preobra['esddsc'];
// 								  		if( $preobra['esdid'] == WF_TIPO_EM_CORRECAO ){
// 									  		$docid = prePegarDocid($preobra['preid']);
					
// 											$sql = "SELECT 
// 														max(hd.htddata) 
// 													FROM workflow.historicodocumento hd 
// 													WHERE 
// 														docid = {$docid} 
// 														AND aedid = 516";
											
// 											$data = $db->pegaUm($sql);	
											
// 											$arData = explode(" ", $data);
						
// 											$dataFinal = calculaPrazoDiligencia( $preobra['preid'] );
// 											$arDataInicial = explode("-",$arData[0]);
// 											$dataInicial = $arDataInicial[2]."/".$arDataInicial[1]."/".$arDataInicial[0];
// 											$arDataFinal = explode("-",$dataFinal);
// 											$dataFinal = $arDataFinal[2]."/".$arDataFinal[1]."/".$arDataFinal[0];
											
// 								  			$sql = "SELECT dioqtddia, diodata FROM par.diligenciaobra WHERE dioativo = 't' AND preid = ".$preobra['preid'];
// 											$dados = $db->pegaLinha( $sql );

// 											if(is_array($dados) && $dados['diodata'] && $dados['dioqtddia']) {
// 												$arData = explode('-', $dados['diodata']);
										
// 												$ano = $arData[0];
// 												$mes = $arData[1];
// 												$dia = $arData[2];
// 												$dataFinal = mktime(24*$dados['dioqtddia'], 0, 0, $mes, $dia, $ano);
// 												$dataFormatada = date('d/m/Y',$dataFinal);
// 												$dataInicial = $dia . "/" . $mes . "/" . $ano;
// 												$dataFinal = $dataFormatada; 
// 											}
											
// 											echo "<label style=\"color: red\"> (Inicio: ".$dataInicial.". Prazo Final: ".$dataFinal.")";
// 											//echo "<label style=\"color: red\"> (Inicio: ".$dataInicial.". Prazo Final: 31/12/2011)";
// 								  		}
// 								  	}else{
// 								  		echo 'Em Análise';
// 								  	}  
                                    echo $preobra['estado1'];
								  	?>
								</td>
								<?php //}?>
							<td>
							<? 
							/* Trecho que preenche a coluna termo */
							$sql = "SELECT terassinado, to_char(terdatainclusao,'dd/mm/YYYY') as terdatainclusao, tcp.arqid FROM par.termoobra teo 
									INNER JOIN par.termocompromissopac tcp ON teo.terid = tcp.terid 
									WHERE teo.preid = '".$preobra['preid']."' AND terstatus = 'A'";
							
							$termoobra = $db->pegaLinha($sql);
							
							if($termoobra) {
								if($termoobra['terassinado']=="f") {
									echo "Gerado (".($termoobra['terdatainclusao']).")";	
								} elseif($termoobra['terassinado']=="t") {
									echo "Assinado";
								}
								if($termoobra['arqid']) {
									echo " <a href=\"par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&download=S&arqid=".$termoobra['arqid']."\" ><img align=absmiddle src=../imagens/anexo.gif border=0></a>";
								}
							} else {
								echo "Não gerado";	
							}
							
							
							/* FIM - Trecho que preenche a coluna termo */
							?>
							</td>
							<td>
							<?
							/* Trecho que preenche a coluna empenho */
							$sql = "SELECT emp.empnumero, vve.vrlempenhocancelado as empvalorempenho FROM par.empenhoobra emo 
									INNER JOIN par.empenho emp ON emo.empid = emp.empid and eobstatus = 'A' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
									inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
									WHERE emo.preid = '".$preobra['preid']."'";
							
							$empenhoobra = $db->pegaLinha($sql);
							
							if($empenhoobra) {
								if($empenhoobra['empnumero']) {
									echo "Gerado (R$".number_format($empenhoobra['empvalorempenho'],2,",",".")." - ".$empenhoobra['empnumero'].")";	
								} else {
									echo "Não gerado";
								}
							} else {
								echo "Não gerado";	
							}
							/* FIM - Trecho que preenche a coluna empenho */
							?>
							</td>
							<td>
							<?
							/* Substituido dia 23/12/2013 parnumseqob ->   pagnumeroob
							 * Analista: Daniel Areas Programador Eduardo Duniceu
							* */
							/* Trecho que preenche a coluna pagamento */
							$sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento 
									FROM par.empenhoobra emo 
									INNER JOIN par.empenho emp ON emp.empid = emo.empid and eobstatus = 'A' and empstatus = 'A' 
									INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
									INNER JOIN par.pagamento pag ON pag.empid = emo.empid   
									WHERE emo.preid = '".$preobra['preid']."' AND pag.pagstatus='A'";
							
							$pagamentoobra = $db->pegaLinha($sql);
							
							if($pagamentoobra) {
								echo "Pago<br>
									  Valor pagamento(R$): ".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."<br> 
									  Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob']."<br> 
									  Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
									  Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
									  ";	
							} else {
								echo "Não pago";	
							}
							/* FIM - Trecho que preenche a coluna pagamento */
							?>
							</td>
							<td>
								<?php
									$sql = "select 
												MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
												MIN(pag.pagdatapagamentosiafi) + 720 as prazo
											from 
												par.pagamentoobra po 
											inner join par.pagamento pag ON pag.pagid = po.pagid and pagstatus = 'A'
											where 
												po.preid = ".$preobra['preid'];
									
									$dataPrimeiroPagamento = $db->carregar($sql);
									if($dataPrimeiroPagamento[0]['data_primeiro_pagamento']){
										$sql = "SELECT popdataprazoaprovado, arqid FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND popvalidacao = 't' AND preid = ".$preobra['preid'];
										$prorrogado = $db->pegaLinha($sql);
										if( $prorrogado['popdataprazoaprovado'] ){
											echo formata_data($prorrogado['popdataprazoaprovado']);
											if( $prorrogado['arqid'] ){ ?>
												<img src="../imagens/consultar.gif" class="consultaprorrogacao" style="cursor:pointer" id="<?php echo $prorrogado['arqid'] ?>" />
											<?php }
										} else {
											echo formata_data($dataPrimeiroPagamento[0]['prazo']);
										}
									}
								?>
							</td>
							<td><?=$prazo; ?></td>
							<td>
								<?php

									$sql = "select distinct
												(
												    CASE WHEN (
													REPLACE( REPLACE( REPLACE(po.ppasaldoconta, ',', ''), '.', ''), '-', '0')::FLOAT +
													REPLACE( REPLACE( REPLACE(po.ppasaldofundos, ',', ''), '.', ''), '-', '0')::FLOAT + 
													REPLACE( REPLACE( REPLACE(po.ppasaldopoupanca, ',', ''), '.', ''), '-', '0')::FLOAT + 
													REPLACE( REPLACE( REPLACE(po.ppasaldordbcdb, ',', ''), '.', ''), '-', '0')::FLOAT
												    ) > 0 THEN
													'<span class=\"processoDetalhes processo_detalhe\" >'
												    ELSE
													'<span class=\"processo_detalhe\" >'
												    END
												) ||
												to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>' as pronumeroprocesso
											from 
												par.processoobraspaccomposicao ppc
											inner join par.processoobra poc on poc.proid = ppc.proid and poc.prostatus = 'A'
											inner join obras2.pagamentopac po on poc.pronumeroprocesso = po.ppaprocesso
											where 
												ppc.pocstatus = 'A' and
												preid = ".$preobra['preid'];

									
									$processo = $db->pegaUm($sql);
								
									echo $processo?$processo:'<center>--</center>';
								?>
							</td>
							</tr>
							<?php $x++ ?>
						<?php endforeach; ?>		
					<?php endif; ?>				
				</tbody>
				<tfoot>
					<tr>
						<td colspan="15" class="SubTituloEsquerda" style="padding-left:15px;">
							<?php //if($boData){
							//if(($db->testa_superuser()&&$booQtdObrasEstado)){ ?>
							<!--  div>
								<span class="<?= $escrita ? 'adicionar' : ''; ?>" style="cursor:pointer; " >
									<img src="../imagens/gif_inclui<?= $escrita ? '' : '_d'; ?>.gif" border="0" align="absmiddle" /> Inserir obra
								</span>
							</div -->
							<?php
//								}
//							} 
							?>
						</td>
					</tr>	
					<tr class="SubTituloEsquerda">
						<td colspan="15"><b>Total de Obras:</b>&nbsp;&nbsp;<?php echo count($arPreObrasComCobertura); ?></td>
					</tr>	
				</tfoot>
			</table>
		</td>
	</tr>
</table>
<?php 
}
if(count($arPreObrasCobertura)>0){
monta_titulo( 'Coberturas', $descricao  );?>
<table class="tabela" style="width:95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align="left" valign="top" width="90%">
			<table class="tabela" style="width:100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<thead>			
					<tr bgcolor="#e9e9e9" align="center">
						<td width="80px" align="center"><b>Prioridade</b>&nbsp<img src="../imagens/ajuda.png" onclick="focamensagem()" width="13px" style="cursor:pointer" /></td>
						<td width="55px" align="center"><b>Ação</b></td>
						<td align="left"><b>Nome da obra</b></td>
						<td align="left"><b>Tipo de obra</b></td>
						<td align="left"><b>Situação</b></td>
						<td align="left"><b>Termo</b></td>
						<td align="left"><b>Empenho processo</b></td>
						<td align="left"><b>Pagamento</b></td>
						<td align="left"><b>Fim da vigência</b></td>
						<td align="left"><b>Prorrogar (dias)</b></td>
						<td align="left"><b>Processo</b></td>
					</tr>
				</thead>
				<tbody id="itensComposicaoComQuadraCobertura">
					<?php $x=0; ?>
					<?php if(count($arPreObrasCobertura)): ?>
						<?php foreach($arPreObrasCobertura as $preobra): ?>
							<?php
							$docid = prePegarDocid($preobra['preid']);
							$esdid = prePegarEstadoAtual($docid); 				
							$cor = ($x % 2) ? "#F7F7F7" : "white";
							
							$seta_cima = '../imagens/seta_cima.gif';
							$styleCima = 'style="cursor:pointer;"';
							if($x == 0){
								$styleCima = '';
								$seta_cima = '../imagens/seta_cimad.gif';
							}
							
							$seta_baixo = '../imagens/seta_baixo.gif';
							$styleBaixo = 'style="cursor:pointer;"';
							if(($x + 1) == count($arPreObrasCobertura)){
								$styleBaixo = '';
								$seta_baixo = '../imagens/seta_baixod.gif';
							}
							
							?>					
							<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';" id="<?php echo $preobra['preid'];?>">
								<td >
									<span><strong><?php echo str_pad($preobra['preprioridade'], 3, "0", STR_PAD_LEFT); ?></strong></span>&nbsp;&nbsp;&nbsp;
									<img class="baixoComCobertura" src="<?php echo $seta_baixo; ?>" <?php echo $styleBaixo ?> title="Diminuir Prioridade" />&nbsp;
									<img class="cimaComCobertura" src="<?php echo $seta_cima; ?>" <?php echo $styleCima ?> title="Aumentar Prioridade" />
								</td>				
								<td style="padding-left:15px;">
									<?php if($esdid == WF_TIPO_EM_CADASTRAMENTO): ?>
<!--										<img class="excluir" src="../imagens/excluir.gif" id="<?php echo $preobra['preid'] ?>" style="cursor:pointer;" />&nbsp;&nbsp;-->
										<img src="../imagens/alterar.gif" class="alterar" style="cursor:pointer" id="<?php echo $preobra['preid'] ?>" />
									<?php else: ?>
										<!-- img src="../imagens/excluir_01.gif" />&nbsp;&nbsp; -->
										<img src="../imagens/alterar.gif" class="alterar" style="cursor:pointer" id="<?php echo $preobra['preid'] ?>" />
									<?php endif; ?>
									<?php $data_primeiro_pagamento = $db->pegaUm("SELECT 
																					MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento
																				FROM 
																					par.pagamentoobra po 
																				INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
																				WHERE 
																					po.preid = ".$preobra['preid']);
									// necessita pedara stoid = esdid
									
//									$sql = "SELECT
//												sto.stoid
//											FROM
//												obras.preobra pre
//											INNER JOIN territorios.municipio 	mun ON mun.muncod = pre.muncod
//											LEFT  JOIN obra s.obr ainfraestrutura oi 
//												INNER JOIN obras.situacaoobra sto ON sto.stoid = oi.stoid
//											ON oi.preid = pre.preid
//											WHERE
//												pre.preid = ".$preobra['preid'];
												
									$sql = "SELECT
												doc.esdid
											FROM
												obras.preobra pre
											INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
											LEFT  JOIN obras2.obras obr 
												INNER JOIN workflow.documento doc ON doc.docid = obr.docid
											ON obr.preid = pre.preid
											WHERE
												pre.preid = {$preobra['preid']}";
									
									$verificaSituacao = $db->pegaUm( $sql );
									
									/*$arrEstadoObra = Array(
															OBR_ESDID_ETAPA_CONCLUIDA, 
															OBR_ESDID_EM_ELABORACAO_DE_PROJETOS, 
															OBR_ESDID_EM_EXECUCAO, 
															OBR_ESDID_EM_LICITACAO, 
															OBR_ESDID_EM_REFORMULACAO, 
															OBR_ESDID_PARALISADA);*/
										/*
										 * A pedido do Francisco dia 10/12/2013
										*/
										$arrEstadoObra = Array(); //  Array(OBR_ESDID_CONCLUIDA);
									
//									if( $podeProrrogar && $data_primeiro_pagamento && ($verificaSituacao == 12 || $verificaSituacao == 99 || $verificaSituacao == 1 || $verificaSituacao == 5 || $verificaSituacao == 9 || $verificaSituacao == 2 ) ):
									$sqlPrazo= "SELECT
													coalesce(MAX(popdataprazoaprovado)::date, (MIN(pag.pagdatapagamentosiafi) + 720)::date ) - now()::date as prazo
												FROM
													par.pagamentoobra po
												INNER JOIN par.pagamento 			pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
												LEFT  JOIN obras.preobraprorrogacao pop ON pop.preid = po.preid AND popstatus = 'A'
												WHERE
													po.preid = {$preobra['preid']}";
									
									$prazo = $db->pegaUm($sqlPrazo);
									
									//$venceuVigenciaDoTermo =  verificaVigenciaVenciada($preobra['preid']);
									// Demanda 950 - A pedido do Tiago Randuz
									$venceuVigenciaDoTermo = true;
									
									if( $podeProrrogar && $data_primeiro_pagamento && !in_array( $verificaSituacao, $arrEstadoObra) && $venceuVigenciaDoTermo ){
										if( $esdid != WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO && $prazo < 91 ){ ?>
										<br/><button type="button" class="prorrogar" id="<?php echo $preobra['preid'] ?>" style="font-size: 9px;">
											<?php echo "Solicitar Prorrogação<br/>da Vigência" ?>	
										</button>	
									<?php }
									} else {
										if( in_array($verificaSituacao, array(OBR_ESDID_CONCLUIDA)) && (in_array( PAR_PERFIL_ADM_OBRAS,$perfil ) || in_array( PAR_PERFIL_SUPER_USUARIO,$perfil ) ) && $esdid != WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO ){
											$sql = "SELECT 
													    sum(po.pobpercentualpag)
													FROM 
													    par.pagamentoobra po 
													INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
													WHERE 
													    po.preid = {$preobra['preid']}
													    and pag.pagsituacaopagamento not ilike '%CANCELADO%'";
											$percent = $db->pegaUm($sql);
											
											if( $percent < 100 ){?>
												<br/><button type="button" class="prorrogar" id="<?php echo $preobra['preid'] ?>" style="font-size: 9px;"><?php echo "Solicitar Prorrogação<br/>da Vigência" ?></button><?
											}
										}
									} ?>
								</td>				
								<td>						
									<a class="alterar" id="<?php echo $preobra['preid'] ?>"><?php echo  $preobra['predescricao'] ?></a>
									<?php if($obPreObraControle->verificaFotosObras($preobra['preid'])): ?>
										<a href="javascript:void(0)">
											<img class="anexos" id="foto_<?php echo $preobra['preid'] ?>" align="absmiddle" border="0" src="../imagens/cam_foto.gif" title="Fotos cadastradas" />
										</a>							
									<?php endif; ?>
									<?php if($obPreObraControle->verificaAnexoObras($preobra['preid'])): ?>
										<a href="javascript:void(0)">
											<img class="anexos" id="documento_<?php echo $preobra['preid'] ?>" align="absmiddle" border="0" src="../imagens/clipe.gif" title="Documentos anexos" />
										</a>							
									<?php endif; ?>													
								</td>
								<td>
									<?php echo $preobra['ptodescricao'] ?>							
								</td>
								<?php //if(in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)){?>
								<td>
									<?php 
									/* Retirado a pedido do Tiago Radunz no dia 22/07/2014 */
// 								  	if( $preobra['esdid'] == WF_TIPO_EM_CADASTRAMENTO || 
// 								  		$preobra['esdid'] == WF_TIPO_OBRA_APROVADA || 
// 								  		$preobra['esdid'] == WF_TIPO_OBRA_ARQUIVADA || 
// 								  		$preobra['esdid'] == WF_TIPO_EM_CORRECAO){
// 								  		echo $preobra['esddsc'];
// 								  		if( $preobra['esdid'] == WF_TIPO_EM_CORRECAO ){
// 									  		$docid = prePegarDocid($preobra['preid']);
					
// 											$sql = "SELECT 
// 														max(hd.htddata) 
// 													FROM workflow.historicodocumento hd 
// 													WHERE 
// 														docid = {$docid} 
// 														AND aedid = 516";
											
// 											$data = $db->pegaUm($sql);	
											
// 											$arData = explode(" ", $data);
						
// 											//$dataFinal = calculaPrazoDiligencia( $arData[0], $preobra['preid'] );
// 											$dataFinal = calculaPrazoDiligencia( $preobra['preid'] );
// 											$arDataInicial = explode("-",$arData[0]);
// 											$dataInicial = $arDataInicial[2]."/".$arDataInicial[1]."/".$arDataInicial[0];
// 											$arDataFinal = explode("-",$dataFinal);
// 											$dataFinal = $arDataFinal[2]."/".$arDataFinal[1]."/".$arDataFinal[0];
											
// 								  			$sql = "SELECT dioqtddia, diodata FROM par.diligenciaobra WHERE dioativo = 't' AND preid = ".$preobra['preid'];
// 											$dados = $db->pegaLinha( $sql );

// 											if(is_array($dados) && $dados['diodata'] && $dados['dioqtddia']) {
// 												$arData = explode('-', $dados['diodata']);
										
// 												$ano = $arData[0];
// 												$mes = $arData[1];
// 												$dia = $arData[2];
// 												$dataFinal = mktime(24*$dados['dioqtddia'], 0, 0, $mes, $dia, $ano);
// 												$dataFormatada = date('d/m/Y',$dataFinal);
// 												$dataInicial = $dia . "/" . $mes . "/" . $ano;
// 												$dataFinal = $dataFormatada; 
// 											}
											
// 											echo "<label style=\"color: red\"> (Inicio: ".$dataInicial.". Prazo Final: ".$dataFinal.")";
// 											//echo "<label style=\"color: red\"> (Inicio: ".$dataInicial.". Prazo Final: 31/12/2011)";
// 								  		}
// 								  	}else{
// 								  		echo 'Em Análise';
// 								  	}  
                                    echo $preobra['estado1']
								  	?>
								</td>
								<?php //}?>
							<td>
							<? 
							/* Trecho que preenche a coluna termo */
							$sql = "SELECT terassinado, to_char(terdatainclusao,'dd/mm/YYYY') as terdatainclusao, tcp.arqid FROM par.termoobra teo 
									INNER JOIN par.termocompromissopac tcp ON teo.terid = tcp.terid 
									WHERE teo.preid = '".$preobra['preid']."' AND terstatus = 'A'";
							
							$termoobra = $db->pegaLinha($sql);
							
							if($termoobra) {
								if($termoobra['terassinado']=="f") {
									echo "Gerado (".($termoobra['terdatainclusao']).")";	
								} elseif($termoobra['terassinado']=="t") {
									echo "Assinado";
								}
								if($termoobra['arqid']) {
									echo " <a href=\"par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&download=S&arqid=".$termoobra['arqid']."\" ><img align=absmiddle src=../imagens/anexo.gif border=0></a>";
								}
							} else {
								echo "Não gerado";	
							}
							
							
							/* FIM - Trecho que preenche a coluna termo */
							?>
							</td>
							<td>
							<?
							/* Trecho que preenche a coluna empenho */
							$sql = "SELECT emp.empnumero, vve.vrlempenhocancelado as empvalorempenho FROM par.empenhoobra emo 
									INNER JOIN par.empenho emp ON emo.empid = emp.empid and eobstatus = 'A' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
									inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
									WHERE emo.preid = '".$preobra['preid']."'";
							
							$empenhoobra = $db->pegaLinha($sql);
							
							if($empenhoobra) {
								if($empenhoobra['empnumero']) {
									echo "Gerado (R$".number_format($empenhoobra['empvalorempenho'],2,",",".")." - ".$empenhoobra['empnumero'].")";	
								} else {
									echo "Não gerado";
								}
							} else {
								echo "Não gerado";
							}
							/* FIM - Trecho que preenche a coluna empenho */
							?>
							</td>
							<td>
							<?
							/* Substituido dia 23/12/2013 parnumseqob ->   pagnumeroob
							 * Analista: Daniel Areas Programador Eduardo Duniceu
							* */
							/* Trecho que preenche a coluna pagamento */
							$sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento 
									FROM par.empenhoobra emo 
									INNER JOIN par.empenho emp ON emp.empid = emo.empid and eobstatus = 'A' and empstatus = 'A' 
									INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso  and pro.prostatus = 'A'
									INNER JOIN par.pagamento pag ON pag.empid = emo.empid   
									WHERE emo.preid = '".$preobra['preid']."' AND pag.pagstatus='A'";
							
							$pagamentoobra = $db->pegaLinha($sql);
							
							if($pagamentoobra) {
								echo "Pago<br>
									  Valor pagamento(R$): ".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."<br> 
									  Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob']."<br> 
									  Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
									  Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
									  ";	
							} else {
								echo "Não pago";	
							}
							/* FIM - Trecho que preenche a coluna pagamento */
							?>
							</td>
							<td>
								<?php
									$sql = "SELECT 
												MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
												MIN(pag.pagdatapagamentosiafi) + 720 as prazo
											FROM 
												par.pagamentoobra po 
											INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
											WHERE 
												po.preid = ".$preobra['preid'];
									
									$dataPrimeiroPagamento = $db->carregar($sql);
									if($dataPrimeiroPagamento[0]['data_primeiro_pagamento']){
										$sql = "SELECT popdataprazoaprovado, arqid FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND popvalidacao = 't' AND preid = ".$preobra['preid'];
										$prorrogado = $db->pegaLinha($sql);
										if( $prorrogado['popdataprazoaprovado'] ){
											echo formata_data($prorrogado['popdataprazoaprovado']);
											if( $prorrogado['arqid'] ){ ?>
												<img src="../imagens/consultar.gif" class="consultaprorrogacao" style="cursor:pointer" id="<?php echo $prorrogado['arqid'] ?>" />
											<?php }
										} else {
											echo formata_data($dataPrimeiroPagamento[0]['prazo']);
										}
									}
								?>
							</td>
							<td> <?=$prazo; ?></td>
							<td>
								<?php

									$sql = "select distinct
												(
												    CASE WHEN (
													REPLACE( REPLACE( REPLACE(po.ppasaldoconta, ',', ''), '.', ''), '-', '0')::FLOAT +
													REPLACE( REPLACE( REPLACE(po.ppasaldofundos, ',', ''), '.', ''), '-', '0')::FLOAT + 
													REPLACE( REPLACE( REPLACE(po.ppasaldopoupanca, ',', ''), '.', ''), '-', '0')::FLOAT + 
													REPLACE( REPLACE( REPLACE(po.ppasaldordbcdb, ',', ''), '.', ''), '-', '0')::FLOAT
												    ) > 0 THEN
													'<span class=\"processoDetalhes processo_detalhe\" >'
												    ELSE
													'<span class=\"processo_detalhe\" >'
												    END
												) ||
												to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>' as pronumeroprocesso
											from 
												par.processoobraspaccomposicao ppc
											inner join par.processoobra poc on poc.proid = ppc.proid and poc.prostatus = 'A'
											inner join obras2.pagamentopac po on poc.pronumeroprocesso = po.ppaprocesso
											where 
												ppc.pocstatus = 'A' and
												preid = ".$preobra['preid'];

									
									$processo = $db->pegaUm($sql);
								
									echo $processo?$processo:'<center>--</center>';
								?>
							</td>	
							</tr>
							<?php $x++ ?>
						<?php endforeach; ?>		
					<?php endif; ?>				
				</tbody>
				<tfoot>
					<tr>
						<td colspan="15" class="SubTituloEsquerda" style="padding-left:15px;">
							<?php //if($boData){
							//if(($db->testa_superuser()&&$booQtdObrasEstado)){
							?>
							<!-- div>
								<span class="<?= $escrita ? 'adicionar' : ''; ?>" style="cursor:pointer; " >
									<img src="../imagens/gif_inclui<?= $escrita ? '' : '_d'; ?>.gif" border="0" align="absmiddle" /> Inserir obra
								</span>
							</div -->
							<?php
//								}	
//							} 
							?>
						</td>
					</tr>	
					<tr class="SubTituloEsquerda">
						<td colspan="15"><b>Total de Obras:</b>&nbsp;&nbsp;<?php echo count($arPreObrasCobertura); ?></td>
					</tr>							
				</tfoot>
			</table>
		</td>
	</tr>
</table>

<?php } ?>

<?php } ?>


<div id="divDebug"></div>