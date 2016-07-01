<?php

include  APPRAIZ."par/classes/Habilita.class.inc";

ini_set("memory_limit","1000M");
set_time_limit(0);

if($_REQUEST['prpid']) {
	$_SESSION['par_var']['prpid'] = $_REQUEST['prpid']; 
}

if($_REQUEST['inuid']) {
	$_SESSION['par_var']['inuid'] = $_REQUEST['inuid']; 
}

if($_REQUEST['requisicao']) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if(!$_SESSION['par_var']['prpid']) die("<script>
											alert('Processo não encontrado');
											window.close();
										</script>");
$processo = $_REQUEST['processo'];

$retornoEmpenhoSigef = verificaEmpenhoSigef( $processo, 'PAR' );

$retorno = verificaEmpenhoValorDivergente($processo, '', 'PAR');

$arrBloqueioObra = verificaBloqueioObras( $_REQUEST['inuid'] );

// Monta tarja vermelha com aviso de pendência de obras
montarAvisoCabecalho(null, null, null, $_REQUEST['inuid']);

$largura 	= "250px";
$altura 	= "120px";
$id 		= "div_auth";

/*$wsusuario = 'MECTIAGOT';
$wssenha   = 'M3135689';*/
?>
<html>
	<head>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		
		<link rel='stylesheet' type='text/css' href='../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<!--                Componente combo box chosen-->
                <script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
                <script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
                <script src="../library/chosen-1.0.0/docsupport/prism.js" type="text/javascript"></script>
                <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
		<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
		<script type="text/javascript" src="../includes/JQuery/maskMoney.js" ></script>
		<script language="JavaScript" src="../estrutura/js/funcoes.js"></script>
		
		<script type="text/javascript" src="./js/par.js"></script>
		<style>
			#listaSub100Empenhada thead tr td, #listaSubASeremEmpenhada thead tr td, #listaRestricoes thead tr td
			{
				background-color: #F7F7F7;
				font-weight: bold;
				font-size: 12px;
				font-family: arial;
			}
			
			.popup_alerta
			{
				width:<?php echo $largura ?>;
				height:<?php echo $altura ?>;
				position:absolute;
				z-index:0;
				top:50%;
				left:50%;
				margin-top:-<?php echo $altura/2 ?>;
				margin-left:-<?php echo $largura/2 ?>;
				border:solid 2px black;
				background-color:#FFFFFF;
				display:none
			}
			.popup_alerta2
			{
				width:90%;
				height:90%;
				position:absolute;
				z-index:0;
				top:50%;
				left:30%;
				margin-top:-250;
				margin-left:-250;
				border:solid 2px black;
				background-color:#FFFFFF;
				display:none
			}
			.popup_alerta3
			{
				width:60%;
				height:60%;
				position:absolute;
				z-index:0;
				top:50%;
				left:30%;
				margin-top:-250;
				margin-left:-250;
				border:solid 2px black;
				background-color:#FFFFFF;
				display:none
			}
                        
                        .comboBusca a.chosen-single{
                            -webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;
                            background-color: #FFFFFF !important;
                            border: 1px solid #AAAAAA !important;
                            border-radius: 5px !important;
                            box-shadow: 0 0 3px #FFFFFF inset, 0 1px 1px rgba(0, 0, 0, 0.1) !important;
                            color: #444444 !important;
                            display: block !important;
                            height: 30px !important;
                            line-height: 29px !important;
                            overflow: hidden !important;
                            padding: 0 0 0 8px !important;
                            position: relative !important;
                            text-align: center !important;
                            text-decoration: none !important;
                            white-space: nowrap !important;

                            /*background-color: red !important;*/
                            /*border: 1px solid #AAAAAA !important;*/
                            /*color: #444444 !important;*/
                            /*text-align: center !important;*/
                        }
		</style>
		<script type="text/javascript">
			function mostraPendencia()
			{
				$( "#dialog_pendencia" ).show();
				$( '#dialog_pendencia' ).dialog({
						resizable: false,
						width: 900,
						height: 600,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Fechar': function() {
								$( this ).dialog( 'close' );
							}
						}
				});
			}
			$(document).ready(function() {
				carregaTelaEmpenhoDivergente();

				jQuery("input.maskMoney").maskMoney({showSymbol:true, symbol:"", decimal:",", thousands:".", allowNegative: true});
				
				carregarListaSubacao();
				var processo = document.getElementById('processo').value;
				
				carregarListaEmpenhoProcesso( '<? echo $processo ?>' );

				jQuery('.focuscampo').focusin(function(){
					if( parseInt( jQuery(this).val() ) == '0' || parseInt( jQuery(this).val() ) == '00' || parseInt( jQuery(this).val() ) == '0,00' || parseInt( jQuery(this).val() ) == '0.00' ){
						jQuery(this).val('');
					}
				});

				jQuery('.focuscampo').focusout(function(){
					if( jQuery(this).val() == '' ){
						jQuery(this).val('0,00');
					}
				});
			});
			
			function reforcoSub(sbdid){
				$.ajax({
					type: "POST",
					url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
					data: "requisicao=solicitarReforco&tipo=visualiza&sbdid="+sbdid,
					async: false,
					success: function(msg){
						document.getElementById('visReforco').style.display='block';
						document.getElementById('visDadoReforco').innerHTML=msg;
				}
				});
			}
			
			function selecionaReforco(sbdid, empnumero){
				document.getElementById('visReforco').style.display='none';
				document.getElementById('reforcosbdid').value=sbdid;
				document.getElementById('reforcoempnumero').value=empnumero;
				document.getElementById('div_auth2').style.display='block'
			}
			
			function selecionaReforco2(){
				sbdid = document.getElementById('reforcosbdid').value;
				empnumero = document.getElementById('reforcoempnumero').value;

				var processo = document.getElementById('processo').value;
				
				var form = document.getElementById('formEmpenharSubacoes');
				
				var usuario = document.getElementById('wsusuario2').value;
				var senha = document.getElementById('wssenha2').value;
				var fonte = form.fonte.value;
				
				if(!sbdid) {
					alert('Nenhuma Subação selecionada');
					return false;
				}
				
				if(!usuario) {
					alert('Favor informar o usuário!');
					return false;
				}
				
				if(!senha) {
					alert('Favor informar a senha!');
					return false;
				}
			
				divCarregando();
				
				var dadosSubacao = $('#formEmpenharSubacoes').serialize();
				//var filtrosobras = $('#formEmpenharSubacoes').serialize();
			
				$.ajax({
			   		type: "POST",
			   		url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
			   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarReforcoEmpenho&sbdid=" + sbdid + "&empnumero=" + empnumero,
			   		async: false,
			   		success: function(msg){
			   			
			   			$( "#dialog-confirm" ).html(msg);
			   		//	$( "#dialog:ui-dialog" ).dialog( "destroy" );
			
						$( "#dialog-confirm" ).dialog({
							resizable: true,
							height:400,
							width:400,
							modal: true,
							buttons: {
								"OK": function() {
									$( this ).dialog( "close" );
									window.location.reload();
									
								}
								
							}
						});
			   		}
				});
				
				carregarListaSubacao();
				carregarListaEmpenhoProcesso( '<? echo $processo ?>' );
			
				divCarregado();
				
			}

			function carregarListaSubacao() {
				$.ajax({
			   		type: "POST",
			   		url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
			   		data: "requisicao=listaSubacao",
			   		async: false,
			   		success: function(msg){
			   			document.getElementById('divListaSubacoes').innerHTML = msg;
			   			if(msg.search("checkbox") < 0 ){
			   				//document.getElementById('formulario').innerHTML = "";
			   			}
			   		}
				});
				
			}
			
			function visEmp(){
				var dados = $('#formEmpenharSubacoes').serialize();
				
				$.ajax({
					type: "POST",
					url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
					data: "requisicao=solicitarEmpenho&tipo=visualiza&"+dados,
					async: false,
					success: function(msg){
						document.getElementById('visXML').style.display='block';
						document.getElementById('visXMLDadosEmpenho').innerHTML=msg;
				}
				});
			}
		
			function marcarChk(obj) {
				var linha = obj.parentNode.parentNode;
			
				document.getElementById('sbdidH').value = obj.value;
				
				if(obj.checked) {
					document.getElementById('id_'+obj.value).className="normal";
					document.getElementById('id_'+obj.value).readOnly=false;
					document.getElementById('id_vlr_'+obj.value).className="normal";
					document.getElementById('id_vlr_'+obj.value).readOnly=false;
					document.getElementById('id_vlr_'+obj.value).focus();
					obj.focus();
				} else {
					document.getElementById('id_'+obj.value).className="disabled";
					document.getElementById('id_'+obj.value).readOnly=true;
					document.getElementById('id_vlr_'+obj.value).className="disabled";
					document.getElementById('id_vlr_'+obj.value).readOnly=true;
				}
				
				var tabela = obj.parentNode.parentNode.parentNode;
				calcularTotal(tabela);
				
				//faço o AJAX para poder carregar o combo da fonte de recurso
				$.ajax({
					type: "POST",
					url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
					data: "requisicao=carregarFonteRecurso&sbdid="+obj.value,
					async: false,
					success: function(msg){
                                            document.getElementById('fonteRecursoSPAN').innerHTML=msg;
                                            carregarChosen('.chosen-select3');
				}
				});

				//faço o AJAX para poder carregar o combo do Plano Interno
				$.ajax({
					type: "POST",
					url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
					data: "requisicao=carregarPlanoInterno&sbdid="+obj.value,
					async: false,
					success: function(msg){
                                            document.getElementById('planointernoSPAN').innerHTML=msg;
                                            carregarChosen('.chosen-select');
				}
				});
				
				filtraPTRES( document.getElementById('planointerno').value );
			}
			
			function filtraPTRES(plicod) {
				sbdid = document.getElementById('sbdidH').value;
				$.ajax({
					type: "POST",
					url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
					data: "requisicao=carregarPtres&plicod="+plicod+"&sbdid="+sbdid,
					success: function(msg){
                                            document.getElementById("ptresSPAN").innerHTML = msg;
                                            carregarChosen('.chosen-select2');
					}
				});
			}
			
			function filtraFonteRecurso(ptres) {
				sbdid = document.getElementById('sbdidH').value;
				$.ajax({
					type: "POST",
					url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
					data: "requisicao=carregarFonteRecursoPTRES&ptres="+ptres,
					success: function(msg){
                                            document.getElementById("fonteRecursoSPAN").innerHTML = msg;
                                            carregarChosen('.chosen-select3');
					}
				});
			}
			
			function calcularTotal(tbl) {
				
				var total=0;
				for(var i=0;i<tbl.rows.length;i++) {
					var input = tbl.rows[i].cells[0].childNodes[0];
					if(input) {
						if(input.checked) {
							total = total + parseFloat(replaceAll(replaceAll(replaceAll(tbl.rows[i].cells[7].childNodes[0].value, '<br>', ''), '.', ''), ',', '.'));
						}
					}
				}
				document.getElementById('id_total').value= mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
			}
			
			function calculaEmpenho(obj) {
				var linha = obj.parentNode.parentNode;
				var valor = parseFloat(replaceAll(replaceAll(replaceAll(linha.cells[3].innerHTML, '<br>', ''), '.', ''), ',', '.')); //valor da subação
				var tabela = obj.parentNode.parentNode.parentNode;
				
				//var valorEmpenhado = linha.cells[4].childNodes[2].value;
				if( obj.id.substr( 0,6 ) == 'id_vlr' ){ //veio do valor
					var total = obj.value;
					var total_mac = mascaraglobal('###.###.###.###,##',replaceAll(total,'.',''));
					var total_for = parseFloat(replaceAll(replaceAll(total, '.', ''), ',', '.'));
					if( total_for > valor ){
						alert( "O valor do empenho não pode ultrapassar o valor da subação." );
						linha.cells[6].childNodes[0].value = '0.00';
						obj.value = mascaraglobal('###.###.###.###,##',replaceAll('0.00','.',''));
						calcularTotal(tabela);
						return false;
					}
					var percent = total_for*100/valor;
					linha.cells[6].childNodes[0].value = percent;
					linha.cells[6].childNodes[1].value = total_mac;
				} else { // veio da porcentagem
					var total = valor*obj.value/100;
					var total_mac = mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
					//linha.cells[7].innerHTML = total_mac;
					linha.cells[7].childNodes[0].value = total_mac;
					linha.cells[6].childNodes[1].value = total_mac;

					if( total > valor ){
						alert( "O valor do empenho não pode ultrapassar o valor da subação." );
						//linha.cells[6].childNodes[1].value = '0.00';
						linha.cells[7].childNodes[0].value = '0.00';
						obj.value = mascaraglobal('###.###.###.###,##',replaceAll('0.00','.',''));
						calcularTotal(tabela);
						return false;
					}
				}
				//linha.cells[5].childNodes[1].value = total;
				
				calcularTotal(tabela);
			}
			
			function verificaPreenchimentoPorcentagem(obj){
			//alert(obj);
			// 0 - &nbsp; 1- Nº do Empenho 2- Subação 3 - Valor da Subação 4 - % Empenhado 5 - Valor Empenhado 6 - % a Empenhar 7 - Valor a Empenhar
				var linha 			= obj.parentNode.parentNode;
				var valorSubacao 	= parseFloat(replaceAll(replaceAll(replaceAll(linha.cells[3].innerHTML, '<br>', ''), '.', ''), ',', '.')); //valor da subação
				var valorInformado 	= linha.cells[7].childNodes[0].value;
				var valorEmpenhado 	= linha.cells[5].innerHTML;
				
				valorEmpenhado = valorEmpenhado.replace("<br>", ""); 
				valorEmpenhado = retiraPontos(valorEmpenhado);
				valorInformado = parseFloat(valorInformado) + parseFloat(valorEmpenhado);
				
				if( parseFloat(valorInformado) > parseFloat(valorSubacao) ){
					alert('O valor informado para empenho ultrapassa 100% do valor da Subação.');
					linha.cells[7].childNodes[0].value = '0,00';
					linha.cells[6].childNodes[0].value = 0;

					var tabela = obj.parentNode.parentNode.parentNode;
					calcularTotal(tabela);
				}
				
			}
			
			function solicitarEmpenhoPar() {
				
				var form = document.getElementById('formEmpenharSubacoes');
				
				var usuario = document.getElementById('wsusuario').value;
				var senha = document.getElementById('wssenha').value;
				var processo = document.getElementById('processo').value;
				var vrlTotalEmpenho = document.getElementById('id_total').value;
				var fonte = form.fonte.value;
				
				var marcado=false;
				for(var i=0;i<form.elements.length;i++) {
					if(form.elements[i].type) {
						if(form.elements[i].type == "checkbox" && form.elements[i].checked == true) {
							marcado = true;
						}
					}
				}
				if(!marcado) {
					alert('Nenhuma Subação selecionada');
					return false;
				}
				
				if(!usuario) {
					alert('Favor informar o usuário!');
					return false;
				}
				
				if(!senha) {
					alert('Favor informar a senha!');
					return false;
				}
				
				if(!fonte){
					alert('Favor informar a Fonte de Recurso');
					return false;
				}
				
				if( parseFloat(vrlTotalEmpenho) < 1 || vrlTotalEmpenho == '' ){
					alert('O Valor total de empenho não pode ser zero.');
					return false;
				}
			
				divCarregando();
				
				var dadosSubacao = $('#formEmpenharSubacoes').serialize();
				//var filtrosobras = $('#formEmpenharSubacoes').serialize();
			
				jQuery.ajax({
			   		type: "POST",
			   		url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
			   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarEmpenho&"+dadosSubacao,
			   		async: false,
			   		success: function(msg){

			   			jQuery( "#dialog-confirm" ).html(msg);

			   		//	$( "#dialog:ui-dialog" ).dialog( "destroy" );
			
						jQuery( "#dialog-confirm" ).dialog({
							resizable: true,
							height:400,
							width:400,
							modal: true,
							buttons: {
								"OK": function() {
									jQuery( this ).dialog( "close" );
									//window.location.reload();
									
								}
								
							}
						});
			   		}
				});
				
				carregarListaSubacao();
				carregarListaEmpenhoProcesso( '<? echo $processo ?>' );
			
				divCarregado();
				
			}
			
			function cancelarEmpenhoWS() {

				var wsusuario = document.getElementById('wsusuario').value;
				var wssenha = document.getElementById('wssenha').value;
				var wsempid = document.getElementById('wsempid').value;
				var wsprocesso = document.getElementById('processo').value;
				var wsespecie = document.getElementById('ws_especie').value;
				
				if(!wsusuario){
					alert('Favor informar o nome de usuário!');
					return false;
				}
				if(!wssenha){
					alert('Favor informar a senha!');
					return false;
				}
				
				//document.getElementById('div_auth_cancela').style.display = 'none';
				
				divCarregando();
				
				$.ajax({
			   		type: "POST",
			   		url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
			   		data: "requisicao=cancelarEmpenho&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha + "&wsespecie="+wsespecie,
			   		async: false,
			   		success: function(msg){
			   		//document.getElementById('debug').innerHTML = msg;
			   		alert(msg);}
				});
				carregarListaSubacao();
				carregarListaEmpenhoProcesso( wsprocesso );
				
				divCarregado();
				
			}
			
			function cancelarEmpenho(empid, processo, especie) {
			
				/* document.getElementById('ws_usuario_consulta').value;
				document.getElementById('ws_senha_consulta').value;
				document.getElementById('ws_empid').value = empid;
				document.getElementById('ws_processo').value = processo;
				document.getElementById('ws_especie').value = especie;
				
				document.getElementById('div_auth_cancela').style.display = 'block';
				document.body.scrollTop = 0; */

				document.getElementById('wsempid').value = empid;
				document.getElementById('ws_especie').value = especie;
				telaLogin( 'cancelar' );
			}
			
			function reduzirEmpenho(empid, processo, especie) {
				window.open('par.php?modulo=principal/reduzirEmpenho&acao=A&empid='+empid+'&processo='+processo+'&especie='+especie, 
		        'reduzirEmpenho', 
		        "height=400,width=800,scrollbars=yes,top=0,left=0" );
			}
			
			function consultarEmpenho(empid,processo) {
			
				/* document.getElementById('ws_usuario_consulta').value;
				document.getElementById('ws_senha_consulta').value;
				document.getElementById('ws_empid').value = empid;
				document.getElementById('ws_processo').value = processo;
				
				document.getElementById('div_auth_consulta').style.display = 'block';
				document.body.scrollTop = 0; */
				document.getElementById('wsempid').value = empid;
				telaLogin( 'consultar' );
			}
			
			
			function consultarEmpenhoWS() {
			
				var wsusuario = document.getElementById('wsusuario').value;
				var wssenha = document.getElementById('wssenha').value;
				var wsempid = document.getElementById('wsempid').value;
				var wsprocesso = document.getElementById('processo').value;
				
				if(!wsusuario){
					alert('Favor informar o nome de usuário!');
					return false;
				}
				if(!wssenha){
					alert('Favor informar a senha!');
					return false;
				}
				
				//document.getElementById('div_auth_consulta').style.display = 'none';
				
				divCarregando();
				
				$.ajax({
			   		type: "POST",
			   		url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
			   		data: "requisicao=consultarEmpenho&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
			   		async: false,
			   		success: function(msg){
			   			//document.getElementById('debug').innerHTML = msg;
			   			alert(msg);
			   		}
				});
				
				carregarListaSubacao();
				carregarListaEmpenhoProcesso( wsprocesso );
				
				divCarregado();
				
			}
			
			function listarSubacao(sbaid, ano, inuid)
			{
				var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&anoatual=" + ano + "&inuid=" + inuid;
				janela(local,800,600,"Subação");
			}
			
			function verificaValor(campo){
				alert(campo.value);
			}

			function aderirPregao(sbdid, inuid)
			{

				divCarregando();
				if( confirm('Tem certeza que deseja aderir ao pregão?') ){
					
					$.ajax({
				   		type: "POST",
				   		url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
				   		data: "requisicao=aderirPregao&sbdid="+sbdid+"&inuid="+inuid,
				   		async: false,
				   		success: function(msg){
				   			
				   			
				   			$( "#dialog-confirm" ).html(msg);
				
							$( "#dialog-confirm" ).dialog({
								resizable: true,
								height:400,
								width:400,
								modal: true,
								buttons: {
									"OK": function() {
										$( this ).dialog( "close" );
										window.location.reload();
										
									}
									
								}
							});
				   		}
					});
					
					carregarListaEmpenhoProcesso( '<? echo $processo ?>' );
					
				}
				divCarregado();
			}
                        
	function carregarChosen(classe){
        var jq = jQuery.noConflict();
    	jq(classe).chosen({allow_single_deselect:true});
	}

                        function telaLogin( action ) {
                        	jQuery('input[type="text"], input[type="password"]').css('font-size', '14px');
                        	
                        	/* jQuery.ajax({
                        		type: "POST",
                        		url: window.location.href,
                        		async: false,
                        		success: function(msg){ */
                        			jQuery( "#div_login" ).show();
                        			//jQuery( "#mostra_dialog" ).html(msg);
                        			jQuery( '#div_login' ).dialog({
                        				resizable: false,
                        				width: 500,
                        				modal: true,
                        				show: { effect: 'drop', direction: "up" },
                        				buttons: {
                        					'Ok': function() {
                        						if( action == 'solicitar' ){
                        							solicitarEmpenhoPar();
                        						}
                        						if( action == 'consultar' ){
                        							consultarEmpenhoWS();
                        						}
                        						if( action == 'cancelar' ){
                        							cancelarEmpenhoWS();
                        						}
                        						jQuery( this ).dialog( 'close' );
                        					},
                        					'Fechar': function() {
                        						jQuery( this ).dialog( 'close' );
                        					}
                        				}
                        			});
                        		/* }
                        	}); */
                        }

		</script>
	</head>
	<body>
		<?php
			monta_titulo( $titulo_modulo, '&nbsp;' );
			cabecalhoSolicitacaoEmpenhoPar();
            exibirHistoricoSigef($processo);
		?>
				<?php 
			$idInuIdRest = $_SESSION['par_var']['inuid'];
			$today = date('Y-m-d');
			if($idInuIdRest)
			{
				$sqlRestricoes = 
					"SELECT 
						res.resdescricao,
						tpr.tprdescricao as tipo_restricao,
						CASE WHEN res.restemporestricao THEN
							to_char(resdatainicio, 'DD/MM/YYYY')
						ELSE
							'-'
						END
						as data_inicio,
						
						CASE WHEN res.restemporestricao THEN
							to_char(resdatafim, 'DD/MM/YYYY')
						ELSE
							'-'
						END
						as data_fim
					FROM
						par.restricaofnde res
					INNER JOIN
						par.tiporestricaofnde tpr ON res.tprid = tpr.tprid
					INNER JOIN
						par.instrumentounidaderestricaofnde iur ON res.resid = iur.resid
					WHERE
						res.resstatus = 'A'
					AND
						iur.inuid = {$idInuIdRest}
					AND
						CASE WHEN res.restemporestricao THEN
							CASE
								WHEN '{$today}' between resdatainicio and resdatafim THEN
									true
								ELSE
									false
								END
						ELSE
							true
						END
					";
				
				$arrRestricoes = $db->carregar($sqlRestricoes);
		if( (count($arrRestricoes) && is_array($arrRestricoes)) || ($arrBloqueioObra['boBloqueia']) ) 
		{
		?> 
		<table id="listaRestricoes" align="center" border="0" style="border-color: #CCCCCC; border-right: 1px solid #CCCCCC;border-style: solid;border-width: 1px;   font-size: xx-small;text-decoration: none; width: 95%;" cellpadding="3" cellspacing="1">
			<thead>
				<tr>
					<td  colspan="4"> Restrições do município </td>
				</tr>

			</thead>
			<tbody>
			
				<?php
				if( count($arrRestricoes) && is_array($arrRestricoes))
				{	
					foreach($arrRestricoes as $kRes => $vRes )
					{
						echo "
							<tr style=\"color:red;\">
								<td style=\"width:40%;\" > {$vRes['resdescricao']} </td>
							</tr>
							";	
					}
				}
				if( $arrBloqueioObra['boBloqueia'] ){
						
						echo "
							<tr style=\"color:red;\">
								<td style=\"width:40%;\" >";
						echo '<span style="color: red;">Existem pendências no Sistema '.$arrBloqueioObra['sistema'].'.</span><a href="#" onclick="mostraPendencia();";><span style="color: blue; cursor:pointer"> Clique aqui para verificar as pendências.</span></a>';
						echo " 	</td>
							 </tr>
								";	
						
					}
				?>
			</tbody>
			</table>
<?php }

}
?>
		
		<div class="demo">
			<div id="dialog-confirm" title="Resposta da solicitação de Empenho:"></div>
		</div>

		
		</div>
		<form id="formEmpenharSubacoes">
		<div id="divListaSubacoes">Carregando...</div>

		<table id="total" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<input type="hidden" id="sbdidH" name="sbdidH" value="">
			<tr>
				<td class="SubTituloDireita"><b>Valor total de empenho:</b></td>
				<td><input type="text" id="id_total" name="name_total" size="30" class="normal" style="text-align:right;" readonly="readonly" value="0,00"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="30%"><b>Plano Interno:</b></td>
				<td>
					<span id="planointernoSPAN">
					<?php
					$sql = "SELECT 	DISTINCT
									plinumplanointerno as codigo,
									plinumplanointerno as descricao
							FROM
								par.planointerno
							WHERE 1 = 2";
					$db->monta_combo( "planointerno", $sql, 'S', 'Selecione', '', '' );
					?>
					</span>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><b>PTRES:</b></td>
				<td>
                                    <div style="margin-top: 10px;" class="comboBusca">
					<span id="ptresSPAN">
					<?php
                                            $sql = "select 
                                                        pliptres as codigo,
                                                        pliptres as descricao
                                                from par.planointerno
                                                where 1 = 2";
                                            $db->monta_combo("ptres", $sql, 'S', 'Selecione', '', '',null,null,null,null,null,null,null,'data-placeholder="Escolha um PTRES"', 'chosen-select2' );
					?>
					</span>
                                    </div>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><b>Fonte de Recurso:</b></td>
				<td>
                                    <div style="margin-top: 10px;" class="comboBusca">
					<span id="fonteRecursoSPAN">
					<?php
					$sql = "SELECT fonte as codigo, fonte as descricao FROM financeiro.empenhopar WHERE ptres = '0'";
					$db->monta_combo( "fonte", $sql, 'S', 'Selecione', '', '', null,null,null,null,null,null,null,null,null);
					?>
					</span>
                                    </div>
				</td>
			</tr>
			<?php if( in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) || in_array( PAR_PERFIL_ADMINISTRADOR, pegaArrayPerfil($_SESSION['usucpf']) ) || in_array( PAR_PERFIL_EMPENHADOR, pegaArrayPerfil($_SESSION['usucpf']) )  ){ ?>
			<tr>
				<td class="SubTituloDireita"><b>Centro de Gestão:</b></td>
				<td>
					<select name='gestaosolicitacao' id="gestaosolicitacao" class="CampoEstilo" style="width: auto">
						<option value="61400000000" selected="selected">61400000000</option>
						<option value="61500000000" >61500000000</option>
						<option value="61700000000">61700000000</option>
						<option value="69500000000">69500000000</option>
					</select>
				</td>
			</tr>
			<?php $itrid = $db->pegaUm( "SELECT iu.itrid FROM par.instrumentounidade iu INNER JOIN par.processopar prp ON prp.inuid = iu.inuid and prp.prpstatus = 'A' WHERE prp.prpid = ".$_SESSION['par_var']['prpid'] );
					if( $itrid == 1 || $_REQUEST['inuid'] == 1 ){ ?>
					<tr>
						<td class="SubTituloDireita"><b>Natureza de Despesa:</b></td>
						<td>
								<select name='naturezadespesasolicitacao' id="naturezadespesasolicitacao" class="CampoEstilo" style="width: auto">
									<option value="44304200">Capital Estados - 44304200</option>
									<option value="33304100">Custeio Estados - 33304100</option>
								</select>	
						</td>
					</tr>
				<?php } else { ?>
					<tr>
						<td class="SubTituloDireita"><b>Natureza de Despesa:</b></td>
						<td>
								<select name='naturezadespesasolicitacao' id="naturezadespesasolicitacao" class="CampoEstilo" style="width: auto">
									<option value="44404200" selected="selected">Capital Município - 44404200</option>
									<option value="33404100" >Custeio Município - 33404100</option>
								</select>	
						</td>
					</tr>
			<?php } ?>
			
			<?php }else{ ?>
			<tr>
				<td></td>
				<td>
					<input name='gestaosolicitacao' id="gestaosolicitacao" value="61400000000"  type="hidden"  />
				</td>
			</tr>
			<?php } ?>
			<tr>
				<td colspan="2" class="SubTituloDireita" style="text-align: center;">
				<?php 
						$perfil = pegaArrayPerfil($_SESSION['usucpf']);
						
						if(	in_array(PAR_PERFIL_EMPENHADOR, $perfil) ||
							in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
							in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)
						){
							$disabled = '';
						}else{
							$disabled = 'disabled="disabled"';
						}
					?>					
					<input type="button" name="solicitar" <?php echo $disabled; ?> id="solicitar" value="Solicitar empenho" onclick="telaLogin('solicitar')" />
					
					<?php if( in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) || in_array( PAR_PERFIL_ADMINISTRADOR, pegaArrayPerfil($_SESSION['usucpf'])) ){ ?>
						<input type=button id=visualizar name=visualizar  value=Visualizar XML onclick=visEmp(); />
					<?php } ?>				
				</td>
			</tr>
		</table>
		</form>
		<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="subtitulodireita" width="20%" style="font-weight: bold" valign="top">Legenda:
					<input type="hidden" name="processo" id="processo" value="<?php echo $_REQUEST['processo']?>" />
					<input type="hidden" name="wsempid" id="wsempid" value="" />
					<input type="hidden" name="ws_especie" id="ws_especie" value="" />
					<input type="hidden" name="tiposistema" id="tiposistema" value="PAR" />
					<input type="hidden" name="empdivergente" id="empdivergente" value="<?php echo $retorno[0]['processo']; ?>" />
				</td>
				<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='../imagens/historico.png' title='Alterar'>&nbsp;-&nbsp;Histórico Situação de Empenho</td> 
				<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='../imagens/refresh2.gif' title='Alterar'>&nbsp;-&nbsp;Atualizar Situação do Empenho</td>
				<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='../imagens/money_ico.png' title='Excluir'>&nbsp;-&nbsp;Reduzir Valor Empenhado</td>
				<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='../imagens/money_cancel.gif' title='Reduzir'>&nbsp;-&nbsp;Cancelar Nota de Empenho(NE)</td>				
			</tr>
		</table>
		<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td id="listaempenhoprocesso">Carregando...</td>
			</tr>
		</table>
		
		<div id="div_login" title="Tela de Identificação" style="display: none; text-align: center;">
			<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td width="30%" class="SubtituloDireita">Usuário:</td>
					<td><?php echo campo_texto("wsusuario", "S", "S", "Usuário", "25","","","","","","", "id='wsusuario'") ?></td>
				</tr>
				<tr>
					<td class="SubtituloDireita">Senha:</td>
					<td>
						<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" 
							onmouseover="MouseOver(this);" value="" size="26" id="wssenha" name="wssenha">
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					</td>
				</tr>
			</table>
		</div>

		<!-- Div para solicitação de Empenho 
		<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
			<div style="width:100%;text-align:right">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
			</div>
			<div style="padding:5px;text-align:justify;">
				<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td width="30%" class="SubtituloDireita">Usuário:</td>
					<td><?php echo campo_texto("wsusuario","S","S","Usuário","22","","","","","","","id='wsusuario'") ?></td>
				</tr>
				<tr>
					<td class="SubtituloDireita">Senha:</td>
					<td>
						<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$wssenha; ?>" size="23" id="wssenha" name="wssenha">
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					</td>
				</tr>
				<tr>
					<td align="center" bgcolor="#D5D5D5" colspan="2">
						<input type="button" name="btn_enviar" onclick="solicitarEmpenhoPar()" value="ok" />
						<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
					</td>
				</tr>
				</table>
			</div>
		</div> -->
		<!-- Div para solicitação de Reforço de Empenho -->
		<div id="div_auth2" class="popup_alerta <?php echo $classeCSS ?>" >
			<div style="width:100%;text-align:right">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('div_auth2').style.display='none'" />
			</div>
			<div style="padding:5px;text-align:justify;">
				<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<input type="hidden" name="reforcosbdid" id="reforcosbdid" value="">
				<input type="hidden" name="reforcoempnumero" id="reforcoempnumero" value="">
				<tr>
					<td width="30%" class="SubtituloDireita">Usuário:</td>
					<td><?php echo campo_texto("wsusuario2","S","S","Usuário","22","","","","","","","id='wsusuario2'") ?></td>
				</tr>
				<tr>
					<td class="SubtituloDireita">Senha:</td>
					<td>
						<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$wssenha; ?>" size="23" id="wssenha2" name="wssenha2">
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					</td>
				</tr>
				<tr>
					<td align="center" bgcolor="#D5D5D5" colspan="2">
						<input type="button" name="btn_enviar" onclick="selecionaReforco2()" value="ok" />
						<input type="button" name="btn_cancelar" onclick="document.getElementById('div_auth2').style.display='none'" value="cancelar" />
					</td>
				</tr>
				</table>
			</div>
		</div>
		<div id="visXML" class="popup_alerta2 <?php echo $classeCSS ?>" >
			<div style="width:100%;text-align:right">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('visXML').style.display='none'" />
			</div>
			<div style="padding:5px;text-align:justify;">
				<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td width="10%" class="SubtituloDireita">Dados do Empenho:</td>
					<td id="visXMLDadosEmpenho"></td>
				</tr>
				</table>
			</div>
		</div>
		<div id="visReforco" class="popup_alerta3 <?php echo $classeCSS ?>" >
			<div style="width:100%;text-align:right">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('visReforco').style.display='none'" />
			</div>
			<div style="padding:5px;text-align:justify;">
				<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td width="100%" class="SubTituloCentroAzul">Processos:</td>
				</tr>
				<tr>
					<td id="visDadoReforco"></td>
				</tr>
				</table>
			</div>
		</div>
<div id="debug"></div>	
<?php
/*
$largura = "250px";
$altura = "120px";
$id = "div_auth_consulta";	
?>
<style>
	.popup_alerta_consulta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_consulta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" class="SubtituloDireita">Usuário:</td>
				<td><?php echo campo_texto("ws_usuario_consulta","S","S","Usuário","22","","","","","","","id='ws_usuario_consulta'") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_consulta" name="ws_senha_consulta">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td align="center" bgcolor="#D5D5D5" colspan="2">
					<input type="hidden" name="ws_empid" id="ws_empid" value="" />
					<input type="hidden" name="ws_processo" id="ws_processo" value="" />
					<input type="button" name="btn_enviar" onclick="consultarEmpenhoWS()" value="ok" />
					<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
				</td>
			</tr>
		</table>
	</div>
</div>

<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_cancela";
?>
<style>
	.popup_alerta_cancela{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_cancela <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" class="SubtituloDireita">Usuário:</td>
				<td><?php echo campo_texto("ws_usuario_cancela","S","S","Usuário","22","","","","","","","id='ws_usuario_cancela'") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_cancela" name="ws_senha_cancela">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td align="center" bgcolor="#D5D5D5" colspan="2">
					<input type="hidden" name="ws_empid" id="ws_empid" value="" />
					<input type="hidden" name="ws_processo" id="ws_processo" value="" />
					<input type="hidden" name="ws_especie" id="ws_especie" value="" />
					<input type="button" name="btn_enviar" onclick="cancelarEmpenhoWS()" value="ok" />
					<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
				</td>
			</tr>
		</table>
	</div>
	<? */ ?>
	<div id="dialog_pendencia" title="Pendências <?=$arrBloqueioObra['sistema']; ?>" style="display: none" >
	<div style="padding:5px;text-align:justify; font-size: 8pt; color: red;">
	<?
		if(is_array($arrBloqueioObra) && count($arrBloqueioObra))
		{
			foreach ($arrBloqueioObra['pendencia'] as $obra => $pendencia) {
				echo ($obra + 1).' - '.$pendencia;
			}
		}	
	?>
	</div>
</div>
</div>

<div id="div_dialog" title="Histórico de Empenho" style="display: none; text-align: center;">
	<div style="padding:5px;text-align:justify;" id="mostra_dialog"></div>
</div>

<div id="div_divergente" title="Empenho Divergentes" style="display: none; text-align: center;">
	<div style="padding:5px;text-align:justify;" id="mostra_divergente"></div>
</div>

	</body>
</html>
<?php

function pegaCnpj($inuid){
	global $db;
	$cnpj = $db->pegaUm("SELECT prpcnpj FROM par.processopar WHERE prpid = ".$_SESSION['par_var']['prpid']);
	if( $cnpj ){
		return $cnpj;
	} else {
		return $db->pegaUm("SELECT iue.iuecnpj 
		                     FROM par.instrumentounidade iu
		                     inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		                     WHERE
		                     	iu.inuid = {$inuid}
		                     	and iue.iuestatus = 'A'
		                        and iue.iuedefault = true");
	}
}
function solicitarReforco($dados){
	global $db;
	
	$sql = "SELECT 
				sd.sbaid, 
				sd.sbdano,
				sd.sbdid,
				emp.empnumero,
				emp.empvalorempenho,
				empcodigopi,
				empcodigoptres,
				emp.empfonterecurso,
				emp.empcodigonatdespesa,
				emp.empcentrogestaosolic,
				( SELECT '<input type=text id=id_vlr_'||sd.sbdid||' value='||
							((SELECT par.recuperaValorValidadosSubacaoPorAno(sd1.sbaid, sd1.sbdano)::numeric(20,2)) - sum(empvalorempenho)) 
						||' name=name_vlr_'||sd.sbdid||' size=15 onBlur=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" onKeyUp=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" class=\"disabled\" readonly=readonly onfocus=\"this.select();\">'	
							as valorsubacao 
				FROM par.subacaodetalhe sd1 
				INNER JOIN par.empenhosubacao ems ON ems.sbaid = sd1.sbaid AND ems.eobano = sd1.sbdano and eobstatus = 'A'
				INNER JOIN par.empenho emp ON emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
				WHERE 
					sd1.sbdid = sd.sbdid
				GROUP BY
					sd1.sbaid, sd1.sbdano) as valorsubacao
			FROM 
				par.subacaodetalhe sd 
			INNER JOIN par.empenhosubacao ems ON ems.sbaid = sd.sbaid AND ems.eobano = sd.sbdano and eobstatus = 'A'
			INNER JOIN par.empenho emp ON emp.empid = ems.empid and empstatus = 'A' 
			WHERE 
				sd.sbdid = ".$dados['sbdid'];
	/*
	$sql = "SELECT 
				sd.sbdid,
				emp.empnumero,
				'<input type=text id=id_vlr_'||sd.sbdid||' value='||
								((SELECT par.recuperaValorValidadosSubacaoPorAno(sd.sbaid , sd.sbdano)::numeric(20,2)) - sum(emp.empvalorempenho) )
					||' name=name_vlr_'||sd.sbdid||' size=15 onBlur=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" onKeyUp=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" class=\"disabled\" readonly=readonly onfocus=\"this.select();\">'
				AS valorsubacao,
				empcodigopi,
				empcodigoptres,
				emp.empfonterecurso,
				emp.empcodigonatdespesa,
				emp.empcentrogestaosolic
			FROM 
				par.subacaodetalhe sd 
			INNER JOIN par.empenhosubacao ems ON ems.sbaid = sd.sbaid AND ems.eobano = sd.sbdano
			INNER JOIN par.empenho emp ON emp.empid = ems.empid 
			WHERE 
				sd.sbdid = ".$dados['sbdid']."
			GROUP BY
				sd.sbdid, emp.empnumero, sd.sbaid , sd.sbdano,empcodigopi,
				empcodigoptres,
				emp.empfonterecurso,
				emp.empcodigonatdespesa,
				emp.empcentrogestaosolic";
	*/
	$dados = $db->carregar($sql);
	$str = "";
	if( is_array( $dados ) ){
		$str .= "<table class='tabela'>";
		$str .= "<tr>";
		$str .= "<td><b>Ação</b></td>";
		$str .= "<td><b>Nota de Empenho</b></td>";
		$str .= "<td><b>Plano Interno</b></td>";
		$str .= "<td><b>PTRES</b></td>";
		$str .= "<td><b>Fonte de Recurso</b></td>";
		$str .= "<td><b>Natureza de Despesa</b></td>";
		$str .= "<td><b>Centro de Gestão</b></td>";
		$str .= "<td><b>Valor do Reforço</b></td>";
		$str .= "</tr>";
		foreach( $dados as $dado ){
			$str .= "<tr>";
			$str .= "<td><input type='radio' name='select_".$dado['sbdid']."' id='select_".$dado['sbdid']."' value='".$dado['sbdid']."' onclick='selecionaReforco(".$dado['sbdid'].",\"".$dado['empnumero']."\")'></td>";
			$str .= "<td>".$dado['empnumero']."</td>";
			$str .= "<td>".$dado['empcodigopi']."</td>";
			$str .= "<td>".$dado['empcodigoptres']."</td>";
			$str .= "<td>".$dado['empfonterecurso']."</td>";
			$str .= "<td>".$dado['empcodigonatdespesa']."</td>";
			$str .= "<td>".$dado['empcentrogestaosolic']."</td>";
			$str .= "<td>".$dado['valorsubacao']."</td>";
			$str .= "</tr>";
		}
		$str .= "</table>";
	}
	echo $str;
	die();
}
function carregarFonteRecurso($dados){
	global $db;
	
	$sql = "SELECT
				fonte as codigo, 
				fonte || ' - ' || dscfonte as descricao 
			FROM 
				financeiro.empenhopar ep
			INNER JOIN par.subacaodetalhe sd ON sd.sbdptres = ep.ptres
			WHERE
				sbdid = ".$dados['sbdid'];
        echo '<div style="margin-top: 10px;" class="comboBusca">';
            $db->monta_combo( "fonte", $sql, 'S', 'Selecione', '', '',null,null,null,null,null,null,null,'data-placeholder="Escolha uma Fonte Recurso"', 'chosen-select3' );
        echo '</div>';
}

function carregarFonteRecursoPTRES($dados){
	global $db;
	
	$sql = "SELECT
				fonte as codigo, 
				fonte || ' - ' || dscfonte as descricao 
			FROM 
				financeiro.empenhopar ep
			WHERE
				ep.ptres = trim('{$dados['ptres']}')";
	
	echo '<div style="margin-top: 10px;" class="comboBusca">';
            $db->monta_combo( "fonte", $sql, 'S', 'Selecione', '', '',null,null,null,null,null,null,null,'data-placeholder="Escolha uma Fonte Recurso"', 'chosen-select3' );
        echo '</div>';
}

function carregarPlanoInterno( $dados ){
	global $db;
	
	$sql = "SELECT sd.sbdano, s.prgid FROM par.subacaodetalhe sd INNER JOIN par.subacao s ON s.sbaid = sd.sbaid WHERE sd.sbdid = ".$dados['sbdid'];
	$dadosSub = $db->pegaLinha( $sql );
	
	if( $dadosSub['prgid'] ){	
		$sql = "SELECT 	DISTINCT
						plinumplanointerno as codigo,
						plinumplanointerno || ' - ' || pliano as descricao
				FROM
					par.planointerno
				WHERE pliano >= ".$dadosSub['sbdano']."  AND prgid = ".$dadosSub['prgid'];
	} else {
		$sql = array();
	}
	
	$planointerno = $db->pegaUm( "SELECT sbdplanointerno FROM par.subacaodetalhe WHERE sbdid = ".$dados['sbdid'] );

        echo '<div style="margin-top: 10px;" class="comboBusca">';
            $db->monta_combo( "planointerno", $sql, 'S', 'Selecione', 'filtraPTRES', '', '','','N', 'planointerno', false, $planointerno, 'Plano Interno', 'data-placeholder="Escolha um Plano Interno"', 'chosen-select' );
        echo '</div>';
}

function carregarPtres( $dados ){
	global $db;
	
	$sql = "SELECT sbdptres FROM par.subacaodetalhe WHERE sbdid = ".$dados['sbdid'];
	$ptres = $db->pegaUm( $sql );
	
	$sql = "select distinct
				pliptres as codigo,
				pliptres as descricao
			from par.planointerno
			where plinumplanointerno = '{$dados['plicod']}'";
	
        echo '<div style="margin-top: 10px;" class="comboBusca">';
            $db->monta_combo( "ptres", $sql, 'S', 'Selecione...', 'filtraFonteRecurso',"","","","N","","", $ptres,null,'data-placeholder="Escolha um PTRES"', 'chosen-select2');
        echo '</div>';
}

function executarReforcoEmpenho($dados){
	global $db;
		
	$data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	
	if( empty($_SESSION['par_var']['prpid']) ){
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
							REGRA DE NEGÓCIO PARA REFORÇO DE EMPENHO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.
							</div>
						</div>
					 	<br>
						';
			
		return false;
	}
	
	$_SESSION['par_var']['esfera'] = $db->pegaUm("SELECT CASE WHEN itrid = 1 THEN 'estadual' ELSE 'municipal' END as esfera FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
	$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");
	$dadosSubacao = $db->pegaLinha("SELECT 
										sd.sbdplanointerno,
										sd.sbdptres,
										sd.sbdid,
										( SELECT ((SELECT par.recuperaValorValidadosSubacaoPorAno(sd1.sbaid, sd1.sbdano)::numeric(20,2)) - sum(empvalorempenho)) as valorsubacao 
											FROM par.subacaodetalhe sd1 
											INNER JOIN par.empenhosubacao ems ON ems.sbaid = sd1.sbaid AND ems.eobano = sd1.sbdano and eobstatus = 'A'
											INNER JOIN par.empenho emp ON emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
											WHERE 
												sd1.sbdid = sd.sbdid
											GROUP BY
												sd1.sbaid, sd1.sbdano) AS valorsubacao,
										emp.empfonterecurso,
										emp.empcodigopi,
										emp.empcodigoptres,
										emp.empcodigonatdespesa,
										emp.empcentrogestaosolic, 
										emp.empprogramafnde,
										emp.empid
									FROM 
										par.subacaodetalhe sd 
									INNER JOIN par.empenhosubacao ems ON ems.sbaid = sd.sbaid AND ems.eobano = sd.sbdano and eobstatus = 'A'
									INNER JOIN par.empenho emp ON emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
									WHERE 
										sd.sbdid = ".$dados['sbdid']." AND emp.empnumero = '{$dados['empnumero']}'");

		if( $dadosse ){
			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){

        	   	$nu_processo='23034655466200900';
				$co_fonte_recurso_solic="0100000000";
				$co_plano_interno_solic= $dadosSubacao['sbdplanointerno'];
				//$co_ptres_solic="020990";
				$co_ptres_solic = $dadosSubacao['sbdptres'];
				$co_natureza_despesa_solic="44504200";



        	} else {
			
	        	$nu_processo=$dadosse['prpnumeroprocesso'];
				$co_natureza_despesa_solic=$dadosSubacao['empcodigonatdespesa'];
//				$co_fonte_recurso_solic=$dados['fonte'];

	        	if($dadosse['protipo'] == 'P') {
					$co_plano_interno_solic= $dadosSubacao['sbdplanointerno'];
					$co_ptres_solic = $dadosSubacao['sbdptres'];
					$frpfuncionalprogramatica = $funcional;
	        	} else {
					$co_plano_interno_solic= $dadosSubacao['sbdplanointerno'];
					$co_ptres_solic = $dadosSubacao['sbdptres'];
					$frpfuncionalprogramatica = $funcional;
	        	}
        	}
		
        	$nu_cnpj_favorecido = pegaCnpj($_SESSION['par_var']['inuid']);
    /*    	$nu_cnpj_favorecido = $db->pegaUm("SELECT iue.iuecnpj 
	                                       FROM par.instrumentounidade iu
	                                               inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
	                                       WHERE
	                                               iu.inuid = {$_SESSION['par_var']['inuid']}
	                                           and iue.iuestatus = 'A'
	                                           and iue.iuedefault = true");*/
			//$nu_cnpj_favorecido = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);

			if($dadosse['prptipoexecucao'] == 'C'){
		   		$co_programa_fnde="03";
				$an_convenio=$an_exercicio;
				$nu_convenio=$nu_convenio;
				$co_tipo_empenho="5";
				$nu_sistema="2";
		    }else if($dadosse['prptipoexecucao'] == 'T'){
		        $co_programa_fnde= "CM"; // "ED";
		        $an_convenio=null;
				$nu_convenio=null;
				$co_tipo_empenho="3";
				$nu_sistema="7";
		    }
		    
		$arrNumero = explode("NE",$dados['empnumero']);
		$nu_empenho_original=$arrNumero[1];
		$an_exercicio_original=$arrNumero[0];
			
		// total do empenho, calculado na tela
//		$vl_empenho=str_replace(array(".",","),array("","."),$dadosSubacao['valorsubacao']);
		$vl_empenho=$dadosSubacao['valorsubacao'];
		// constante=01
		$co_especie_empenho="02";
		// constante=1
		$co_esfera_orcamentaria_solic="1";
		// constante=2
		$co_observacao="2";

		$co_descricao_empenho="0012";
		// constante=15253
		$co_gestao_emitente="15253";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
		// constante=153173
		$co_unidade_gestora_emitente="153173";
		// constante=26298
		$co_unidade_orcamentaria_solic="26298";
		// nulo
		$nu_proposta_siconv=null;
			
		$co_natureza_despesa_solic 	= $dadosSubacao['empcodigonatdespesa'];
		$co_centro_gestao_solic 	= $dadosSubacao['empcentrogestaosolic'];
		$co_fonte_recurso_solic 	= $dadosSubacao['empfonterecurso'];
		$co_plano_interno_solic		= $dadosSubacao['empcodigopi'];
		$co_ptres_solic 			= $dadosSubacao['empcodigoptres'];
		$co_programa_fnde 			= $dadosSubacao['empprogramafnde'];
			
		$sql = "SELECT distinct l.lwsid FROM par.logws l
					inner join par.historicowsprocessopar h ON l.lwsid = h.lwsid
				WHERE
					h.prpid = {$_SESSION['par_var']['prpid']}
					and h.hwpxmlretorno is null
					and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessopar where prpid = {$_SESSION['par_var']['prpid']})
					and l.lwstiporequest = '$co_especie_empenho'";
		$request_id = $db->pegaUm($sql);
				 
		if( empty($request_id) ){
			$arrParam = array(
					'lwstiporequest' 	=> $co_especie_empenho,
					'usucpf' 			=> $_SESSION['usucpf']
			);
			$request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
		}
		
    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<request_id>$request_id</request_id>
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

		}
		
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}
			
		try {
			$arrParam = array(
					'lwsrequestdata' 	=> 'now()',
					'lwsurl' 			=> $urlWS,
					'lwsmetodo' 		=> 'solicitar',
					'lwsid' 			=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
			$arrParam = array(
					'prpid' 		=> $_SESSION['par_var']['prpid'],
					'lwsid' 		=> $request_id,
					'hwpxmlenvio' 	=> str_replace( "'", '"', $arqXml),
					'hwpdataenvio' 	=> 'now()',
					'usucpf' 		=> $_SESSION['usucpf']
			);
			$hwpid = logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'insert' );
			
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
	
			$xmlRetorno = $xml;
			
			$arrParam = array(
					'lwsresponsedata' 	=> 'now()',
					'lwsid' 			=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
			$arrParam = array(
					'hwpid'			=> $hwpid,
					'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno)
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
	
		    $xml = simplexml_load_string( stripslashes($xml));
	
		    echo "------ REFORÇO DE EMPENHO ------\n\n";
			
			$result = (integer) $xml->status->result;
	
			if($result) {
				$arrParam = array(
						'lwserro' => false,
						'lwsid' => $request_id
				);
				logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
				
				$arrParam = array(
						'hwpid' 		=> $hwpid,
						'hwpwebservice' => 'REFORÇO DE EMPENHO - Sucesso'
				);
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
				
				echo "REFORÇO DE EMPENHO - Sucesso";
				echo $xml->status->result->message->text;
				$hwpwebservice = "REFORÇO DE EMPENHO - Sucesso";
				$db->executar("UPDATE par.subacaodetalhe SET sbdreforco='FALSE' WHERE sbdid=".$dados['sbdid']);
				/* $db->executar("INSERT INTO par.empenhoreforco(usucpf, empid, sbdid, reevlr, reeprotocolo, reedata, reestatus)
	    					   VALUES ('".$_SESSION['usucpf']."', '".$dadosSubacao['empid']."', ".$dados['sbdid'].", ".$dadosSubacao['valorsubacao'].", '{$xml->body->nu_seq_ne}', NOW(), 'A');");
				$db->executar("INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
	    					   VALUES ('".$_SESSION['usucpf']."', '".$dadosSubacao['empid']."', NOW(), '$co_especie_empenho', 'REFORÇO');"); */
				
				$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empvalorempenho, empnumeroprocesso, empcodigoespecie, empcodigopi, empcodigoesfera, empcodigoptres,
				            empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
				            empprogramafnde, empnumerosistema, usucpf, empprotocolo, empidpai, empsituacao)
					    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
					    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
					    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
					    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
					            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
					            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
					            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
					            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
					            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
					            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
					            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
					            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
					            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
					            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
					            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
					            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
					            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
					            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
					            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
					            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
					            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
					            '".$_SESSION['usucpf']."',
					            '".$xml->body->nu_seq_ne."',
					            '".$dadosSubacao['empid']."',
					            '2 - EFETIVADO'
					            ) RETURNING empid;";
				
				$empid = $db->pegaUm($sql);
				
				$sql = "SELECT eobid, sbaid, empid, eobpercentualemp, eobvalorempenho, eobano, eobstatus
						FROM par.empenhosubacao WHERE empid = {$dadosSubacao['empid']}";
				$arrEmpSub = $db->carregar($sql);
				$arrEmpSub = $arrEmpSub ? $arrEmpSub : array();
				
				foreach ($arrEmpSub as $v) {
					$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
			    			VALUES ('".$v['sbaid']."', '".$empid."', '".$v['eobpercentualemp']."', '".$v['eobvalorempenho']."', '".$v['eobano']."');";
					$db->executar($sql);
				}
				
				$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    					VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', 'REFORÇO DE EMPENHO');";
				$db->executar($sql);
				
				$db->commit();			
			} else {
				$arrParam = array(
						'lwserro' => true,
						'lwsid' => $request_id
				);
				logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
				
				$arrParam = array(
						'hwpid' 		=> $hwpid,
						'hwpwebservice' => 'REFORÇO DE EMPENHO  - Erro'
				);
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
				
				echo "\n\n\n";
				echo "REFORÇO DE EMPENHO  - Erro";
				echo $xml->status->error->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->error->message->text)."\n\n";
				$hwpwebservice = "REFORÇO DE EMPENHO  - Erro";
				
			}	
		
	} catch (Exception $e){
		
		$arrParam = array(
				'lwserro' 			=> true,
				'lwsresponsedata' 	=> 'now()',
				'lwsid' 			=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO EXECUTAR A SOLICITAÇÃO DE REFORÇO DE EMPENHO: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro-Serviço encontra-se temporariamente indisponível.Favor tente mais tarde.
					</div>
				</div>
			 	<br>
				';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		
		$arrParam = array(
				'hwpid' 		=> $hwpid,
				'hwpwebservice' => 'REFORÇO DE EMPENHO  - Erro',
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno).' - Erro Exception: '.$erroMSG
		);
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );

		//echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO EXECUTAR A SOLICITAÇÃO DE REFORÇO DE EMPENHO: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro: '.$erroMSG.'
					</div>
				</div>
			 	<br>
				';


	}
	
}

function executarEmpenho($dados) {
	global $db;
	
	// if( atualizaValoresSubacao( $dados ) ){

		$_SESSION['par_var']['esfera'] = $db->pegaUm("SELECT CASE WHEN itrid = 1 THEN 'estadual' ELSE 'municipal' END as esfera FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
		$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");
		
		$obHabilita = new Habilita();
		$cnpj 		= pegaCnpj($_SESSION['par_var']['inuid']);
		$habilitado = $obHabilita->consultaHabilitaEntidade($cnpj);	
		
		if($dadosse['prptipoexecucao'] == 'C'){	
				
			if($habilitado == 'Habilitado'){
				$res_acc = atualizaDadosContaCorrentePar( $dados );
				if( $res_acc ){
					$res_sp = solicitarProcesso($dados);
					$res_cc = consultarContaCorrente($dados);
					if(!$res_cc) $res_sc = solicitarContaCorrente($dados);
					$dadosConvenioFNDE = solicitarConvenio($dados);
					$nu_convenio  = (int) $dadosConvenioFNDE->nu_convenio;
					$an_exercicio = (int) $dadosConvenioFNDE->an_exercicio;
					if($dadosConvenioFNDE){
						$res_se = solicitarEmpenho($dados, $nu_convenio ,$an_exercicio );
						if($res_se === true){ // mando para o SIGARP
							//enviarItensSigarp($dados);
						}
					}
				}else{
					
					$res_sp = solicitarProcesso($dados);
					if($res_sp){
						$res_sc = solicitarContaCorrente($dados);
						$dadosConvenioFNDE = solicitarConvenio($dados);
						$nu_convenio  = (int) $dadosConvenioFNDE->nu_convenio;
						$an_exercicio = (int) $dadosConvenioFNDE->an_exercicio;
						if($dadosConvenioFNDE){
							$res_se = solicitarEmpenho($dados, $nu_convenio ,$an_exercicio );
							if($res_se === true){ // mando para o SIGARP
								//enviarItensSigarp($dados);
							}
						}
					}
					
					
				}
			}else{
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
								ERRO AO VERIFICAR A ENTIDADE NO SISTEMA DE HABILITAÇÃO: 
								<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
									Erro: '.$habilitado.'
								</div>
							</div>
						 	<br>
							';
			}
			
		}else{
			//$habilitado = 'Habilitado';
			if($habilitado == 'Habilitado'){
				$res_acc = atualizaDadosContaCorrentePar( $dados );			
				if( $res_acc ){
					$res_sp = solicitarProcesso($dados);
					if($res_sp){
						$res_cc = consultarContaCorrente($dados);
						if($res_cc){
							$res_sc = solicitarContaCorrente($dados);
						}
						
						if($res_sp && ($res_sp || $res_sc)  ){
							$res_se = solicitarEmpenho($dados, null ,null );
							if($res_se === true){ // mando para o SIGARP
								//enviarItensSigarp($dados);
							}
						}
					}
				}else{
					
					$res_sp = solicitarProcesso($dados);
					if($res_sp){
						$res_sc = solicitarContaCorrente($dados);
						if($res_sc && $res_sp ){
							$res_se = solicitarEmpenho($dados, null ,null );
							if($res_se === true){ // mando para o SIGARP
								//enviarItensSigarp($dados);
							}
						}
					}
					
					
				}
			}else{
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
								ERRO AO VERIFICAR A ENTIDADE NO SISTEMA DE HABILITAÇÃO: 
								<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
									Erro: '.$habilitado.'
								</div>
							</div>
						 	<br>
							';
			}
		}
	//} else {
	//	return false;
	// }
}

function atualizaValoresSubacao( $dados ){
	
	global $db;

		// Verifico os itens da subação
		$sql = "SELECT 
					s.sbaid,
					sd.sbdano,
					sic.icoid,
					pic.idsigarp,
					CASE WHEN sbacronograma = 1 --global
					THEN
						CASE WHEN sic.icovalidatecnico IS NULL -- antigos
						THEN coalesce(sic.icoquantidade,0)
						ELSE -- novos
							CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
								sum(coalesce(sic.icoquantidadetecnico,0))
							END
						END
					ELSE -- escolas
						CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
						THEN -- escolas sem itens
							CASE WHEN se.sesvalidatecnico IS NULL -- antigos
								THEN 
									sum(coalesce(se.sesquantidade,0))
								ELSE -- novos
									sum(coalesce(se.sesquantidadetecnico,0))
								END
							ELSE -- escolas com itens
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
								THEN
									sum(coalesce(ssi.seiqtd,0))
								ELSE -- novos
									CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(ssi.seiqtdtecnico,0))
									END
								END
							END
					END as quantidade,
					sic.icovalor as valor
				FROM
					par.subacao s
				INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
			--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid
				LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano
				LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
				LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
				INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
				WHERE
					s.sbastatus = 'A' AND sd.sbdid IN ( ".implode(", ", $dados['chk'])." ) AND 
					
					 CASE WHEN sbacronograma = 1 
                     THEN sic.icovalidatecnico <> 'N' 
                     ELSE 
                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
                        	se.sesvalidatecnico <> 'N'
                        ELSE
                        	sic.icovalidatecnico <> 'N' 
                        END
                     END
                group by
					s.sbaid, sd.sbdano, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor";
			//		pr.prpid = ".$_SESSION['par_var']['prpid'];

		$itens = $db->carregar( $sql );
		
		$it = array();
		$erro = array();
		
		if( is_array($itens) ){
			foreach( $itens as $item ){
				if( !$item['idsigarp'] == '' ){
					$valorSigarp = diponibilizaPregoesItens($item['idsigarp'],'valor');
					if( $valorSigarp ){
						$valor = (string) $valorSigarp;
						$valorSimec = $item['valor'];
						$it[$item['icoid']]['icoid'] = $item['icoid'];
						$it[$item['icoid']]['idsigarp'] = $item['idsigarp'];
						$it[$item['icoid']]['valor'] = $item['valor'];
						$it[$item['icoid']]['valorSigarp'] = $valor;
						if( $valorSimec != $valor ){
							$erro[] = $item['icoid']; 
						}
					}
				}
			}
		} else {
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						Não existem itens aprovados na subação. <BR>
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							 Necessário analisar a subação!
						</div>
					</div>
				 	<br>
					';
			return false;
		}

		if( is_array($erro) && $erro[0] ){ // O valor é diferente então atualizo os valores dos itens.
			foreach( $erro as $err ){
				$sqlAtualizaItens .= " UPDATE par.subacaoitenscomposicao SET icovalor = ".$it[$err]['valorSigarp']." WHERE icoid = ".$it[$err]['icoid']."; ";
			}
			$db->executar( $sqlAtualizaItens );
			$db->commit();
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						VALORES DOS ITENS DE COMPOSIÇÃO DA SUBAÇÃO FORAM ATUALIZADOS DE ACORDO COM O PREGÃO. <BR>
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Solicite novamente o empenho!
						</div>
					</div>
				 	<br>
					';
			
			return false;
		}
		return true;

}

function diponibilizaPregoesItens($item, $tipo){

	global $db;
	
	try {
		
		$data_created = date("c");

		$usuario = USUARIO_SIGARP;
		$senha   = SENHA_SIGARP;
		
		$sql = "SELECT
					CASE WHEN estuf IS NOT NULL 
						THEN estuf 
						ELSE mun_estuf 
					END as estuf 
				FROM
					par.instrumentounidade 
				WHERE 
					inuid = ".$_SESSION['par_var']['inuid'];
		$estuf = $db->pegaUm( $sql );

$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
            <nu_seq_item>{$item}</nu_seq_item>
            <uf>{$estuf}</uf>
        </params>
    </body>
</request>
XML;

	 /*
echo '<pre>';		
echo simec_htmlentities($arqXml);
die();
		*/   		
		if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
//			$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/pregao';
			$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'listar') )
				->execute();
		
		$xmlRetorno = $xml;
		$xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
		
		//print_r ( (string) $xml->body->VL_ITEM );
		//die();

		if(!$result) {
			
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'diponibilizaPregoesItens - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
			
			$erros = $xml->status->error->message->text;
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							'.iconv("UTF-8", "ISO-8859-1", $erros).'
						</div>
					</div>
				 	<br>
					';
			*/
			//echo "*** Descrição do erro ***\n\n";
			//$erros = $xml->status->error->message->text;	
			//echo "* ".iconv("UTF-8", "ISO-8859-1", $erros);
		} else { //sucesso
			
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'diponibilizaPregoesItens - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			if( $tipo == 'valor' ){
				return $xml->body->VL_ITEM ? $xml->body->VL_ITEM : null;
			} else {
				return (integer) $xml->body->NU_SEQ_PREGAO ? $xml->body->NU_SEQ_PREGAO : false;
			}
		}
		
	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Temporariamente indisponível.Favor tente mais tarde.
						</div>
					</div>
				 	<br>
					';
			*/
			
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		/* -- ARRUMAR DEPOIS 
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Erro-WS SIGARP: '.$erroMSG.'
							<br>
							<pre>
							'.print_r($e).'
							</pre>
						</div>
					</div>
				 	<br>
					';
		*/
		
		//echo "Erro-WS SIGARP: $erroMSG";
		//echo "<pre>";
		//print_r($e);
		
	}
}

function aderirPregao( $dados, $tipo = 1 ){
	
	global $db;
	
	$item = $db->carregarColuna( "SELECT icoid FROM par.empenhopregaoitemperdido WHERE sbdid = ".$dados['sbdid'] );
	$sbaid = $db->pegaUm( "SELECT sbaid FROM par.subacaodetalhe WHERE sbdid = ".$dados['sbdid'] );
	
try {
		
		// Verifico os itens da subação
		$sql = "SELECT 
					s.sbaid,
					sd.sbdano,
					sd.sbdid,
					sic.icoid,
					pic.idsigarp,
					CASE WHEN sbacronograma = 1 --global
					THEN
						CASE WHEN sic.icovalidatecnico IS NULL -- antigos
						THEN coalesce(sic.icoquantidade,0)
						ELSE -- novos
							CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
								sum(coalesce(sic.icoquantidadetecnico,0))
							END
						END
					ELSE -- escolas
						CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
						THEN -- escolas sem itens
							CASE WHEN se.sesvalidatecnico IS NULL -- antigos
								THEN 
									sum(coalesce(se.sesquantidade,0))
								ELSE -- novos
									sum(coalesce(se.sesquantidadetecnico,0))
								END
							ELSE -- escolas com itens
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
								THEN
									sum(coalesce(ssi.seiqtd,0))
								ELSE -- novos
									CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(ssi.seiqtdtecnico,0))
									END
								END
							END
					END as quantidade,
					sic.icovalor as valor
				FROM
					par.subacao s
				INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
			--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid
				INNER JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano AND sic.icoid IN ( ".implode(', ', $item)." )
				LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
				LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
				INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
				WHERE
					s.sbastatus = 'A' AND sd.sbdid IN ( ".$dados['sbdid']." ) AND 
					
					 CASE WHEN sbacronograma = 1 
                     THEN sic.icovalidatecnico <> 'N' 
                     ELSE 
                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
                        	se.sesvalidatecnico <> 'N'
                        ELSE
                        	sic.icovalidatecnico <> 'N' 
                        END
                     END
                group by
					s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor";
			//		pr.prpid = ".$_SESSION['par_var']['prpid'];

		$arrItens[$sbaid] = $db->carregar( $sql );

		$cnpj = pegaCnpj($_SESSION['par_var']['inuid']);

		$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");
		
		$dadosTemo = $db->pegaLinha("SELECT dopnumerodocumento, dopdatafimvigencia FROM par.vm_documentopar_ativos WHERE prpid = ".$_SESSION['par_var']['prpid']);
		if(strlen($dadosTemo['dopdatafimvigencia']) == 7){
			$mes = substr($dadosTemo['dopdatafimvigencia'], 0, 2);
			$ano = substr($dadosTemo['dopdatafimvigencia'], 3, 4);
			$vigencia = $ano."-".$mes;
		} else {
			$mes = substr($dadosTemo['dopdatafimvigencia'], 3, 2);
			$ano = substr($dadosTemo['dopdatafimvigencia'], 6, 4);
			$vigencia = $ano."-".$mes;
		}

		$data_created = date("c");
		$usuario = USUARIO_SIGARP;
		$senha   = SENHA_SIGARP;
		
		$itEnviado = array();
		$itPerdido = array();
		$it = 0;
		$cont = 0;
		$montou = false;
		$ultimaSubacao = 0;
		$itens2 = array();
		
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_processo>'.$dadosse['prpnumeroprocesso'].'</nu_processo> 
        	<nu_seq_tipo_pagamento>'.$tipo.'</nu_seq_tipo_pagamento>
        	<nu_termo_compromisso>'.$dadosTemo['dopnumerodocumento'].'</nu_termo_compromisso>
        	<dt_vigencia>'.$vigencia.'</dt_vigencia>';
			foreach( $arrItens as $subacao => $itens ){
        		$monta = 0;
				if( $itens[0] ){
		        	foreach( $itens as $item ){
		        		$itens2[$cont]['sbdid'] = $item['sbdid'];
		        		$itens2[$cont]['icoid'] = $item['icoid'];
		        		$cont++;
		        		if($item['idsigarp']){
			        		$pregao = diponibilizaPregoesItens($item['idsigarp'],'pregao');
			        		if( $item['quantidade'] <> 0 ){
				        		if( $pregao ){
				        			$it++;
			        				$montou = true;
				        			if( $subacao != $ultimaSubacao && $ultimaSubacao != 0 ){
				        				$arqXmlSt .= '
				        							</sub_acao>';
				        			}
				        			$ultimaSubacao = $subacao;
				        			if( $monta == 0 ){
				        				$monta++;
				        				$arqXmlSt .= '
				        							<sub_acao>
				        								<nu_sub_acao>'.$subacao.'</nu_sub_acao>';
				        			}
									$arqXmlSt .= '
										<item>
											<nu_seq_pregao>'.$pregao.'</nu_seq_pregao> 
											<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item> 
											<qt_item>'.$item['quantidade'].'</qt_item>  
											<vl_pagamento>'.$item['valor'].'</vl_pagamento>
										</item>';
									$itEnviado[] = array('sbdid' => $item['sbdid'],
														'icoid' => $item['icoid']);
				        		}
			        		}
		        		}
		        		$pregao = false;
					}
				}
			}
			if( $montou == true ){
        		$arqXmlSt .= '
        					</sub_acao>';
        	}
		$arqXmlSt .= '</params>
    </body>
</request>';

$arqXml = <<<XML
{$arqXmlSt}
XML;

		if( $it == 0 ){ //Não tem nenhum item para enviar!
			echo "WS-Sigarp: Nenhum item para enviar. Verifique os pregões.";
			return true;
		}

 /*
echo '<pre>';		
echo $arqXml;
die();
	*/  		
		if( $_SESSION['baselogin'] == "simec_desenvolvimento" ){
			$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
		}
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();
				
		$xmlRetorno = $xml;
							
		$xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
		
	//	print_r ($xml);
	//	die();

		if(!$result) { //ERRO
			
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'enviarManualmenteItensSigarp - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
			
			
			$erros = $xml->status->error->message->text;
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							'.iconv("UTF-8", "ISO-8859-1", $erros).'
						</div>
					</div>
				 	<br>
					';
			
		} else {
			// Deu certo
			
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'enviarManualmenteItensSigarp - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
			
			$dados = ($xml->body->params);
			foreach( $dados->adesaopregao as $adesao ){
				$sub = (string) $adesao->nu_sub_acao;
				$ano = $arrAno[$sub];
				$ad = (integer) $adesao->nu_seq_solicitacao_adesao;
				foreach( $adesao->item as $item ){
					$it = (integer) $item->nu_seq_item;
					$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus) VALUES ( ".$_SESSION['par_var']['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A');";
				}
			}
			
			if($dados->canceladas){
				foreach( $dados->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
					$sqlCancelamento .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE adesao = ".$adesaocancelada."; ";
				}
				$db->executar($sqlCancelamento);
			}
			
			if( $sqlItensEnviados ){
				$db->executar( $sqlItensEnviados );
			}
			
			if( is_array($itEnviado) && $itEnviado[0] ){
				foreach( $itEnviado as $it ){
					if($it['sbdid'] && $it['icoid'] && $_SESSION['par_var']['prpid'] ){
						$sqlDelete .= " DELETE FROM par.empenhopregaoitemperdido WHERE
									prpid = ".$_SESSION['par_var']['prpid']." AND
									sbdid = ".$it['sbdid']." AND
									icoid = ".$it['icoid']."; ";
					}
				}
				$db->executar($sqlDelete);
				$db->commit();
			}
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						SOLICITAÇÃO DE ADESÃO REALIZADA COM SUCESSO
					</div>
				 	<br>
					';
		}
		
	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.
						</div>
					</div>
				 	<br>
					';
			
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
	
		//echo "Erro-WS Enviar dados para o SIGARP: $erroMSG";
		
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Erro-WS Enviar dados para o SIGARP: '.$erroMSG.'
						</div>
					</div>
				 	<br>
					';
		
	}
	
	
}

function enviarItensSigarp($dados){
	global $db;

	try {
		
		$sql = "SELECT
					sd.sbaid,
					sd.sbdano
				FROM
					par.subacaodetalhe sd
				INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
				WHERE
					sd.sbdid IN (".implode(", ", $dados['chk']).") AND s.sbastatus = 'A'";
		
		$sbaids = $db->carregar($sql);
		
		$arrItens = array();
		foreach( $sbaids as $sba ){
		
			// Verifico os itens da subação
			$sql = "SELECT 
						s.sbaid,
						sd.sbdano,
						sd.sbdid,
						sic.icoid,
						pic.idsigarp,
						CASE WHEN sbacronograma = 1 --global
						THEN
							CASE WHEN sic.icovalidatecnico IS NULL -- antigos
							THEN coalesce(sic.icoquantidade,0)
							ELSE -- novos
								CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
									sum(coalesce(sic.icoquantidadetecnico,0))
								END
							END
						ELSE -- escolas
							CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
							THEN -- escolas sem itens
								CASE WHEN se.sesvalidatecnico IS NULL -- antigos
									THEN 
										sum(coalesce(se.sesquantidade,0))
									ELSE -- novos
										sum(coalesce(se.sesquantidadetecnico,0))
									END
								ELSE -- escolas com itens
									CASE WHEN sic.icovalidatecnico IS NULL -- antigos
									THEN
										sum(coalesce(ssi.seiqtd,0))
									ELSE -- novos
										CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
											sum(coalesce(ssi.seiqtdtecnico,0))
										END
									END
								END
						END as quantidade,
						sic.icovalor as valor
					FROM
						par.subacao s
					INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
				--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid
					LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano
					LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
					LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
					INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
					WHERE
						s.sbastatus = 'A' AND sd.sbaid IN ( ".$sba['sbaid']." ) AND sd.sbdano IN ( ".$sba['sbdano']." ) AND 
						
						 CASE WHEN sbacronograma = 1 
	                     THEN sic.icovalidatecnico <> 'N' 
	                     ELSE 
	                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
	                        	se.sesvalidatecnico <> 'N'
	                        ELSE
	                        	sic.icovalidatecnico <> 'N' 
	                        END
	                     END
	                group by
						s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor
					ORDER BY
						s.sbaid";
				//		pr.prpid = ".$_SESSION['par_var']['prpid'];

			$arrItens[$sba['sbaid']] = $db->carregar( $sql );
			$arrAno[$sba['sbaid']] = $sba['sbdano'];
		}
			
		$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");

		$cnpj = pegaCnpj($_SESSION['par_var']['inuid']);
		
		$dadosTemo = $db->pegaLinha("SELECT dopnumerodocumento, dopdatafimvigencia FROM par.vm_documentopar_ativos WHERE prpid = ".$_SESSION['par_var']['prpid']);
		if(strlen($dadosTemo['dopdatafimvigencia']) == 7){
			$mes = substr($dadosTemo['dopdatafimvigencia'], 0, 2);
			$ano = substr($dadosTemo['dopdatafimvigencia'], 3, 4);
			$vigencia = $ano."-".$mes;
		} else {
			$mes = substr($dadosTemo['dopdatafimvigencia'], 3, 2);
			$ano = substr($dadosTemo['dopdatafimvigencia'], 6, 4);
			$vigencia = $ano."-".$mes;
		}
		/*
		$cnpj = $db->pegaUm("SELECT iue.iuecnpj 
	                                       FROM par.instrumentounidade iu
	                                               inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
	                                       WHERE
	                                               iu.inuid = {$_SESSION['par_var']['inuid']}
	                                           and iue.iuestatus = 'A'
	                                           and iue.iuedefault = true");*/
		//$cnpj = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
		
		$data_created = date("c");
		$usuario = USUARIO_SIGARP;
		$senha   = SENHA_SIGARP;
		
		$itPerdido = array();
		$it = 0;
		$cont = 0;
		$montou = false;
		$ultimaSubacao = 0;
		$itens2 = array();
		
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_processo>'.$dadosse['prpnumeroprocesso'].'</nu_processo> 
        	<nu_seq_tipo_pagamento>1</nu_seq_tipo_pagamento>
        	<nu_termo_compromisso>'.$dadosTemo['dopnumerodocumento'].'</nu_termo_compromisso>
        	<dt_vigencia>'.$vigencia.'</dt_vigencia>';
			foreach( $arrItens as $subacao => $itens ){
        		$monta = 0;
				if( $itens[0] ){
		        	foreach( $itens as $item ){
		        		$itens2[$cont]['sbdid'] = $item['sbdid'];
		        		$itens2[$cont]['icoid'] = $item['icoid'];
		        		$cont++;
		        		if($item['idsigarp']){
			        		$pregao = diponibilizaPregoesItens($item['idsigarp'],'pregao');
			        		if( $item['quantidade'] <> 0 ){
				        		if( $pregao ){
				        			$it++;
			        				$montou = true;
				        			if( $subacao != $ultimaSubacao && $ultimaSubacao != 0 ){
				        				$arqXmlSt .= '
				        							</sub_acao>';
				        			}
				        			$ultimaSubacao = $subacao;
				        			if( $monta == 0 ){
				        				$monta++;
				        				$arqXmlSt .= '
				        							<sub_acao>
				        								<nu_sub_acao>'.$subacao.'</nu_sub_acao>';
				        			}
									$arqXmlSt .= '
										<item>
											<nu_seq_pregao>'.$pregao.'</nu_seq_pregao> 
											<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item> 
											<qt_item>'.$item['quantidade'].'</qt_item>  
											<vl_pagamento>'.$item['valor'].'</vl_pagamento>
										</item>';
				        		} else {
				        			$itPerdido[] = array('sbdid' => $item['sbdid'],
														'icoid' => $item['icoid']);
				        		}
			        		}
		        		}
		        		$pregao = false;
					}
				}
			}
			if( $montou == true ){
        		$arqXmlSt .= '
        					</sub_acao>';
        	}
		$arqXmlSt .= '</params>
    </body>
</request>';
		
//ver(simec_htmlentities($arqXmlSt));
//die();

$arqXml = <<<XML
{$arqXmlSt}
XML;


		if( $it == 0 ){ //Não tem nenhum item para enviar!
			foreach( $itens2 as $item ){ // Salvo todos os itens que estão sendo empenhados!
				if($item['sbdid'] && $item['icoid'] && $_SESSION['par_var']['prpid'] ){
					$sqlInsertItem .= " INSERT INTO par.empenhopregaoitemperdido (
								prpid,
								sbdid,
								icoid )
							VALUES
								(".$_SESSION['par_var']['prpid'].",
								".$item['sbdid'].",
								".$item['icoid']."); ";
				}
			}
			$db->executar($sqlInsertItem);
			$db->commit();
			return true;
		}

		if( $_SESSION['baselogin'] == "simec_desenvolvimento" ){
			$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
		}
		
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();
				
		$xmlRetorno = $xml;
							
		$xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
		
		if(!$result) { //ERRO
			
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'enviarItensSigarp - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
			
			
			$erros = $xml->status->error->message->text;
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							'.iconv("UTF-8", "ISO-8859-1", $erros).'
						</div>
					</div>
				 	<br>
					';
			
			if( is_array($itens2) && $itens2[0] ){
				foreach( $itens2 as $item ){
				if($item['sbdid'] && $item['icoid'] && $_SESSION['par_var']['prpid'] ){
					$sqlInsert .= " INSERT INTO par.empenhopregaoitemperdido (
								prpid,
								sbdid,
								icoid )
							VALUES
								(".$_SESSION['par_var']['prpid'].",
								".$item['sbdid'].",
								".$item['icoid']."); ";
					}
				}
				$db->executar($sqlInsert);
				$db->commit();
			}
			
			/*
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message->text;	
			echo "* ".iconv("UTF-8", "ISO-8859-1", $erros);
			*/
		} else {
			// Deu certo
			
			$dados = ($xml->body->params);
			foreach( $dados->adesaopregao as $adesao ){
				$sub = (string) $adesao->nu_sub_acao;
				$ano = $arrAno[$sub];
				$ad = (integer) $adesao->nu_seq_solicitacao_adesao;
				foreach( $adesao->item as $item ){
					$it = (integer) $item->nu_seq_item;
					$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus) VALUES ( ".$_SESSION['par_var']['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A');";
				}
			}
			
			if($dados->canceladas){
				foreach( $dados->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
					$sqlCancelamento .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE adesao = ".$adesaocancelada."; ";
				}
				$db->executar($sqlCancelamento);
			}
			
			if( $sqlItensEnviados ){
				$db->executar( $sqlItensEnviados );
			}
			
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'enviarItensSigarp - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
			
			if( is_array($itPerdido) && $itPerdido[0] ){
				foreach( $itPerdido as $it ){
					if($_SESSION['par_var']['prpid'] && $it['sbdid'] && $it['icoid'] ){
						$sqlInsert .= " INSERT INTO par.empenhopregaoitemperdido (
									prpid,
									sbdid,
									icoid )
								VALUES
									(".$_SESSION['par_var']['prpid'].",
									".$it['sbdid'].",
									".$it['icoid']."); ";
					}
				}
				$db->executar($sqlInsert);
				$db->commit();
			}
			
			return true;
		}
		
	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.
						</div>
					</div>
				 	<br>
					';
			
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
	
		//echo "Erro-WS Enviar dados para o SIGARP: $erroMSG";
		
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Erro-WS Enviar dados para o SIGARP: '.$erroMSG.'
						</div>
					</div>
				 	<br>
					';
		
	}
}

function solicitarEmpenho($dados, $nu_convenio = null ,$an_exercicio = null) {
	global $db;
	
	if( $dados['chk'][0] == '' ){
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
						ERRO AO TENTAR SOLICITAR EMPENHO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Não foi selecionada nenhuma subação.
						</div>
					</div>
				 	<br>
					';
		exit();
	}
		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';
		
		if( empty($_SESSION['par_var']['prpid']) ){
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
							REGRA DE NEGÓCIO PARA EMPENHO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.
							</div>
						</div>
					 	<br>
						';
			
			return false;
		}

	    $dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");
	    if( $dados['chk'][0] != '' ){
		    $emendas = $db->carregar("SELECT DISTINCT em.emecod
										FROM 
											par.subacaodetalhe sd
										    inner join par.subacaoemendapta se on se.sbdid = sd.sbdid and se.sepstatus = 'A'
										    inner join emenda.emenda em on em.emeid = se.emeid
		    							WHERE sd.sbdid IN (".implode(', ', $dados['chk']).") ");
	    }
	    
	    $totalsubEmenda = count($emendas);

	    if($totalsubEmenda > 1){
		    if(is_array($emendas) && $emendas[0]){
		    	foreach($emendas as $emenda ){
		    		if($emenda['emecod'] == NULL or $emenda['emecod'] == '' ){
		    			 echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							REGRA DE NEGÓCIO PARA EMPENHO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Uma das Subações é de Emenda e outra não. É necessário empenhar separadamente.
							</div>
						</div>
					 	<br>
						';
		    			 
		    			 return false; 
		    		}
		    	}
		    }
	    }
	    
	   
	    $tipoemenda = $emendas[0]['emecod'];
	    // PI
	    if( $dados['chk'][0] != '' ){
		    $sql = "select distinct sbdplanointerno
		    		from par.subacaodetalhe
		    		where sbdid IN (".implode(", ", $dados['chk']).") ";
		    $pis = $db->carregar($sql);
		    $totalPis = count($pis);
	    }
	   if($totalPis > 1){
	   	 //echo "Existe subações com PIs diferente. So pode ser empenhado subações com mesmo PIs.";
	   	 
	   	 echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					REGRA DE NEGÓCIO PARA EMPENHO
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Existe subações com PIs diferente. So pode ser empenhado subações com mesmo PIs.
					</div>
				</div>
			 	<br>
				';
	   	 
	   	 return false;
	   }
	   if(is_array($pis) && $pis[0] ){
	   		foreach($pis as $pi){
	   				if($pi['sbdplanointerno'] == '' || $pi['sbdplanointerno'] == NULL ){
	   					//echo "Existe subações sem PI Cadastrado. É necessário inserir os PIs nas subações.";
	   					echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
								REGRA DE NEGÓCIO PARA EMPENHO
								<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
									Existe subações sem PI Cadastrado. É necessário inserir os PIs nas subações. <br>
									Clique no botão: <img src="../imagens/consultar.gif\" title="Visualizar Subação" border="0"> para visualizar a subação.
								</div>
							</div>
						 	<br>
							';
	   	 				return false;
	   				}
	   		}
	   }
	   
	   if( $dados['chk'][0] != '' ){
		   //PTRES
		   $sql = "select distinct sbdptres
		    		from par.subacaodetalhe
		    		where sbdid IN (".implode(", ", $dados['chk']).") ";
		   $ptres = $db->carregar($sql);
			$totalPtres = count($ptres);
	   }
	   if($totalPtres > 1){
	   	// echo "Existe PIs com PTRES diferente. So pode ser empenhado subações com mesmo PTRES.";
	   	 
	   	 echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
								REGRA DE NEGÓCIO PARA EMPENHO
								<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
									Existe PIs com PTRES diferente. So pode ser empenhado subações com mesmo PTRES.
								</div>
							</div>
						 	<br>
							';
	   	 
	   	 return false;
	   }
	   
	   $PI = $db->pegaLinha("select distinct sbdplanointerno, sbdptres
								from par.subacaodetalhe s 
								inner join par.subacao sb on sb.sbaid = s.sbaid 
								WHERE
									s.sbdid IN (".implode(", ", $dados['chk']).")");
	   
	   
	   // Caso ele mude nas opções eu reconheco o que ele marcou agora.
	   if( $dados['planointerno'] && $dados['ptres'] ){
	   		$PI['sbdplanointerno'] = $dados['planointerno'];
	   		$PI['sbdptres'] = $dados['ptres'];
	   }
	   
		$sql = "SELECT  
					(((((((pt.funcod::text) || pt.sfucod::text)) || pt.prgcod::text)) || pt.acacod::text)) || pt.loccod::text AS fupfuncionalprogramatica
				FROM monitora.pi_planointerno pi
				inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
				inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
				WHERE
					pt.ptres = '".$PI['sbdptres']."'";
		
		$funcional = $db->pegaUm( $sql );
		
		if($dadosse) {
			/*
			$sql = "select distinct sbanaturezadespesa
								from par.subacaodetalhe s 
								inner join par.subacao sb on sb.sbaid = s.sbaid 
								WHERE
									s.sbdid IN (".implode(", ", $dados['chk']).")";

			
			$naturezaDespesa = $db->pegaUm( $sql );
			*/
	        
        	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
        	   $_SESSION['baselogin'] == "simec_espelho_producao" ){

        	   	$nu_processo='23034655466200900';
				$co_fonte_recurso_solic="0100000000";
				$co_plano_interno_solic= $PI['sbdplanointerno'];
				//$co_ptres_solic="020990";
				$co_ptres_solic = $PI['sbdptres'];
				$co_natureza_despesa_solic="44504200";



        	} else {
			
	        	$nu_processo=$dadosse['prpnumeroprocesso'];
				// constante=4042
				$co_natureza_despesa_solic="44404200";
				
				// fonte selecionada pelo usuario no formulario
				$co_fonte_recurso_solic=$dados['fonte'];

	        	if($dadosse['protipo'] == 'P') {
					// constante=MEC00001
					$co_plano_interno_solic= $PI['sbdplanointerno'];
					// constante=037825
					//$co_ptres_solic="024788";
					$co_ptres_solic = $PI['sbdptres'];
					// constante=12365144812KU0001
					//$frpfuncionalprogramatica="14480E53262980001";
					$frpfuncionalprogramatica = $funcional;
	        	} else {
					// constante=MEC00002
					$co_plano_interno_solic= $PI['sbdplanointerno'];
					// constante=037826
					//$co_ptres_solic="024788";
					$co_ptres_solic = $PI['sbdptres'];
					// constante=12365144812KV0001
					//$frpfuncionalprogramatica="14480E53262980001";
					$frpfuncionalprogramatica = $funcional;
	        	}
        	}
        }
        

		
        $nu_cnpj_favorecido = pegaCnpj($_SESSION['par_var']['inuid']);
        /*
        $nu_cnpj_favorecido = $db->pegaUm("SELECT iue.iuecnpj 
	                                       FROM par.instrumentounidade iu
	                                               inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
	                                       WHERE
	                                               iu.inuid = {$_SESSION['par_var']['inuid']}
	                                           and iue.iuestatus = 'A'
	                                           and iue.iuedefault = true");*/
       // $nu_cnpj_favorecido = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
        
		if($_SESSION['par_var']['esfera']=='estadual') {
			$co_natureza_despesa_solic = '44304200';
        }else{
			$co_natureza_despesa_solic = '44404200';
        }
        
        if($_SESSION['par_var']['inuid'] == 1 ){ // DF
			$co_natureza_despesa_solic = '44304200';
        }

		$nu_sistema="7";
		if($dadosse['prptipoexecucao'] == 'C'){
	   		//$co_centro_gestao_solic="61500000000";
	   		$co_programa_fnde="03";
			$an_convenio=$an_exercicio;
			$nu_convenio=$nu_convenio;
			$co_tipo_empenho="5";
	    }else if($dadosse['prptipoexecucao'] == 'T'){
	       // $co_centro_gestao_solic="61700000000";
	        $co_programa_fnde= "CM"; // "ED";
	        $an_convenio=null;
			$nu_convenio=null;
			$co_tipo_empenho="3";
	    }
	    
		// nulo
		$nu_empenho_original=null;
		// nulo
		$an_exercicio_original=null;
		// total do empenho, calculado na tela
		$vl_empenho=str_replace(array(".",","),array("","."),$dados['name_total']);
		// constante=01
		$co_especie_empenho="01";
		// constante=1
		$co_esfera_orcamentaria_solic="1";
		// constante=2
		$co_observacao="2";

		$co_descricao_empenho="0010";
		// constante=15253
		$co_gestao_emitente="15253";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
		// constante=153173
		$co_unidade_gestora_emitente="153173";
		// constante=26298
		$co_unidade_orcamentaria_solic="26298";
		// nulo
		$nu_proposta_siconv=null;
		
		 // Centro de Gestão -  selecionada pelo usuario no formulario
		$co_centro_gestao_solic=$dados['gestaosolicitacao'];
		
		if( !in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) && !in_array( PAR_PERFIL_ADMINISTRADOR, pegaArrayPerfil($_SESSION['usucpf']) ) ){
			// Caso seja uma subação de Emenda o Sistema muda o centro de gestão para o abaixo
			if($tipoemenda){
				$co_centro_gestao_solic="61700000000";
			}
		}
		
		$co_natureza_despesa_solic  = $dados['naturezadespesasolicitacao'];
		
		if($co_plano_interno_solic == 'BS892910806'){
			$co_esfera_orcamentaria_solic = "2";
		}

		// A pedido do WEBER do FNDE - dia 13/12/2013 Gato para o PTRES do MDS
	   if($co_ptres_solic == '067148'){
        	$co_esfera_orcamentaria_solic = "2";
        }
        
        if( $dados['tipo'] != 'visualiza' ){
        	$sql = "SELECT distinct l.lwsid FROM par.logws l
						inner join par.historicowsprocessopar h ON l.lwsid = h.lwsid
					WHERE
						h.prpid = {$_SESSION['par_var']['prpid']}
        				and h.hwpxmlretorno is null
        				and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessopar where prpid = {$_SESSION['par_var']['prpid']})
						and l.lwstiporequest = '$co_especie_empenho'";
        	$request_id = $db->pegaUm($sql);
        	
        	if( empty($request_id) ){
		        $arrParam = array(
		        		'lwstiporequest' 	=> $co_especie_empenho,
		        		'usucpf' 			=> $_SESSION['usucpf'],
		        );
		        $request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
        	}
        }

    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<request_id>$request_id</request_id>
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

	if( $dados['tipo'] == 'visualiza' ){
		echo '<pre>';		
		echo simec_htmlentities($arqXml);
		exit;
    }
    	
//echo $arqXml;
//return false;
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}
		
		$arrParam = array(
				'prpid' 		=> $_SESSION['par_var']['prpid'],
				'lwsid' 		=> $request_id,
				'hwpxmlenvio' 	=> str_replace( "'", '"', $arqXml),
				'hwpdataenvio' 	=> 'now()',
				'usucpf' 		=> $_SESSION['usucpf']
		);
		$hwpid = logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'insert' );
		
	try {
		$arrParam = array(
				'lwsrequestdata' 	=> 'now()',
				'lwsurl' 			=> $urlWS,
				'lwsmetodo' 		=> 'solicitar',
				'lwsid' 			=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
		
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();
		$xmlRetorno = $xml;
		
		$arrParam = array(
				'lwsresponsedata' 	=> 'now()',
				'lwsid' 			=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
		
		$arrParam = array(
				'hwpid'			=> $hwpid,
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno)
		);
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
		
	    $xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
		if(!$result) {
			
			$arrParam = array(
					'lwserro' 	=> true,
					'lwsid' 	=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
			$arrParam = array(
					'hwpid' 		=> $hwpid,
					'hwpwebservice' => 'solicitarEmpenho - Erro'
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
			
			$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
				ERRO AO TENTAR SOLICITAR O EMPENHO:
				<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " > ';
			
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					//echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
					$mensagem .= ' '.iconv("UTF-8", "ISO-8859-1", $err->text).' ';
				}
			}
			$mensagem .='</div>
						</div>
		 				<br>';
			echo $mensagem;

		    return false;
		} else {
			$arrParam = array(
					'lwserro' 	=> false,
					'lwsid' 	=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
			$arrParam = array(
					'hwpid' 		=> $hwpid,
					'hwpwebservice' => 'solicitarEmpenho - Sucesso'
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );

			$sql = "INSERT INTO par.empenho(
            empcnpj,
            empnumerooriginal,
            empanooriginal,
            empvalorempenho,
            empnumeroprocesso,
            empcodigoespecie,
            empcodigopi,
            empcodigoesfera,
            empcodigoptres,
            empfonterecurso,
            empcodigonatdespesa,
            empcentrogestaosolic,
            empanoconvenio,
            empnumeroconvenio,
            empcodigoobs,
            empcodigotipo,
            empdescricao,
            empgestaoeminente,
            empunidgestoraeminente,
            empprogramafnde,
            empnumerosistema,
            usucpf,
            empprotocolo,
            empsituacao
            )
		    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
		    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
		    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
		    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
		            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
		            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
		            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
		            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
		            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
		            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
		            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
		            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
		            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
		            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
		            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
		            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
		            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
		            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
		            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
		            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
		            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
		            '".$_SESSION['usucpf']."',
		            '".$xml->body->nu_seq_ne."',
		            '8 - SOLICITAÇÃO APROVADA'
		            ) RETURNING empid;";

			$empid = $db->pegaUm($sql);

			$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    				VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', '8 - SOLICITAÇÃO APROVADA');";

			$db->executar($sql);

		if($dados['chk']) {
			foreach($dados['chk'] as $sbdid){			
				$valorPorSubacao = retiraPontosBD($dados['name_vlr_'.$sbdid]);
				//$valorPorSubacao = $valor;
				$valorTotalSubacao = $dados['vlrsubacao'][$sbdid];
				//					$valorPorSubacao = str_replace(array(".",","),array("","."),number_format($dados['name_vlr_'.$sbdid], 2, ',', '.'));
				//					$valorPorSubacao = str_replace(array(".",","),array("","."),number_format($dados['vlrTotal_'.$sbdid], 2, ',', '.'));
				//					$valorPorSubacao = str_replace(array(".",","),array("","."),$dados['vlr_'.$sbaid]);
				$sql = "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".$sbdid;
				$linha = $db->pegaLinha( $sql );
				/*	
				$eobvalorempenho = $db->pegaUm("SELECT sum(eobvalorempenho) FROM par.empenhosubacao WHERE sbaid = {$linha['sbaid']} and empid = '{$empid}' and eobano = '{$linha['sbdano']}' and eobstatus = 'A'");			 
					
				if( !empty($eobvalorempenho) ){
					$eobvalorempenho = (float)$valorPorSubacao + (float)$eobvalorempenho;
					$novoPercentEmp = ((float)$eobvalorempenho * 100) / (float)$valorTotalSubacao;
		
					$sql = "UPDATE par.empenhosubacao SET
								eobpercentualemp = {$novoPercentEmp},
								eobvalorempenho = {$eobvalorempenho}
							WHERE
								sbaid = {$linha['sbaid']}
								and eobano = '{$linha['sbdano']}'
								and eobstatus = 'A'";
				} else {
				*/
					$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
		    						VALUES ('".$linha['sbaid']."', '".$empid."', '".$dados['name_'.$sbdid]."', '".$valorPorSubacao."', '".$linha['sbdano']."');";
				//}
				$db->executar($sql);
					
				$sql = "UPDATE par.subacaodetalhe SET sbdptres = '".$co_ptres_solic."' , sbdplanointerno = '".$co_plano_interno_solic."' WHERE sbdid = ".$sbdid;
				$db->executar($sql);
								
				if( $dados['name_'.$sbdid] == '100' ){
					$sql = "UPDATE par.subacaodetalhe SET prpid = ".$_SESSION['par_var']['prpid'].", ssuid = 13 WHERE sbdid = ".$sbdid;
				} else {
					$sql = "UPDATE par.subacaodetalhe SET prpid = ".$_SESSION['par_var']['prpid'].", ssuid = 18  WHERE sbdid = ".$sbdid;
				}
				$db->executar($sql);
				$db->commit();
			}
		}
			
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					EMPENHO EFETIVADO COM SUCESSO 
					</div>
			 	<br>
				';
			
			$db->commit();
			return true;
		}

	} catch (Exception $e){
		
		$arrParam = array(
				'lwserro' 			=> true,
				'lwsresponsedata' 	=> 'now()',
				'lwsid' 			=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO EXECUTAR A SOLICITAÇÃO DE EMPENHO: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro-Serviço encontra-se temporariamente indisponível.Favor tente mais tarde.
					</div>
				</div>
			 	<br>
				';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		
		$arrParam = array(
				'hwpid' 		=> $hwpid,
				'hwpwebservice' => 'solicitarEmpenho - Erro',
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno).' - Erro Exception: '.$erroMSG
		);
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );

		//echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO EXECUTAR A SOLICITAÇÃO DE EMPENHO: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro: '.$erroMSG.'
					</div>
				</div>
			 	<br>
				';


	}
}

function cancelarEmpenho($dados) {
	global $db;

	try {
		$data_created = date("c");
		
		/*$sql = "SELECT count(*) as num FROM par.historicoempenho WHERE empid='".$dados['empid']."' AND empsituacao IN ('2 - EFETIVADO','8 - SOLICITAÇÃO APROVADA')";
		$historicoefetivado = $db->pegaUm($sql);*/

		/* if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
		} else { */
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];
		//}
		
		$dadosemp = $db->pegaLinha("SELECT e.empnumeroprocesso, e.empprogramafnde, e.empcnpj, e.empnumero, e.empvalorempenho, e.empprotocolo, op.prpid, op.prptipoexecucao
			 							FROM par.empenho e
											inner join par.processopar op on e.empnumeroprocesso = op.prpnumeroprocesso and op.prpstatus = 'A' and empstatus = 'A'
										WHERE e.empid = '".$dados['empid']."'");

        if($dadosemp) {
        	$nu_seq_ne = $dadosemp['empprotocolo'];
        }
        $errodecancelamento = false;
      
        $nu_cnpj_favorecido = pegaCnpj($_SESSION['par_var']['inuid']);
        
        $dadosEmpenho = $db->pegaLinha("SELECT * FROM par.empenho WHERE empid = ".$dados['empid']);
        
        if($dadosEmpenho['empnumero']) {
        	$arrNumero = explode("NE",$dadosEmpenho['empnumero']);
        	$nu_empenho_original=$arrNumero[1];
        	$an_exercicio_original=$arrNumero[0];
        		
        	#A pedido da Sâmara via e-mail dia 06/11/2013, que alterasse a espécie empenho de 03 para 13
        	if( $an_exercicio_original == date('Y') ){
        		$co_especie_empenho="03";
        	} else {
        		$co_especie_empenho="13";
        	}
        } else {
        	$nu_empenho_original=null;
        	$an_exercicio_original=null;
        	$co_especie_empenho="13";
       	}
        
       	$vl_empenho=$dadosEmpenho['empvalorempenho'];
       	$co_esfera_orcamentaria_solic="1";
       	$co_observacao="2";
       	$co_descricao_empenho="0011";
       	$co_gestao_emitente="15253";
       	$co_unidade_gestora_emitente="153173";
       	$co_unidade_orcamentaria_solic="26298";
       	$nu_proposta_siconv=null;
       	$co_centro_gestao_solic=$dadosEmpenho['empcentrogestaosolic'];
       	$co_natureza_despesa_solic =$dadosEmpenho['empcodigonatdespesa'];
       	$nu_sistema = $dadosEmpenho['empnumerosistema'];
       	$co_fonte_recurso_solic=$dadosEmpenho['empfonterecurso'];
       	$co_ptres_solic=$dadosEmpenho['empcodigoptres'];
		$co_plano_interno_solic=$dadosEmpenho['empcodigopi'];
   		$nu_processo=$dadosEmpenho['empnumeroprocesso'];
        
		if($dadosemp['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde="03";
	   		$an_convenio=$an_exercicio;
        	$nu_convenio=$nu_convenio;
        	$co_tipo_empenho="5";
        }else if($dadosemp['prptipoexecucao'] == 'T'){
			$co_programa_fnde= "CM"; // "ED";
        	$an_convenio=null;
        	$nu_convenio=null;
        	$co_tipo_empenho="3";
        }
        
        
// SE EXISTE NE
//------------------- CANCELAR EMPENHO ----------------------//
if($nu_seq_ne){

    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'cancelar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;

		if($result) { // Se sucesso			
			
			$status 	= (string)$xml->body->status;
			$co_status	= substr( $status, 0, 1 );

			if( trim($co_status) == 0 ){
				$errodecancelamento = true;
				/* echo "*** Descrição do erro ***\n\n";
				echo $status; */
				$hwpwebservice = 'cancelarEmpenho - Erro';				
			} else {
				$hwpwebservice = 'cancelarEmpenho - Sucesso';
				$errodecancelamento = false;
				echo "------ CANCELAMENTO DE EMPENHO ------\n\n";
				echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
				
				//$db->executar("UPDATE par.empenho SET empsituacao='CANCELADO' WHERE empid='".$dados['empid']."'");
				
				$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empvalorempenho, empnumeroprocesso, empcodigoespecie, empcodigopi, empcodigoesfera, empcodigoptres,
				            empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
				            empprogramafnde, empnumerosistema, usucpf, empprotocolo, empidpai, empsituacao)
					    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
					    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
					    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
					    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
					            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
					            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
					            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
					            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
					            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
					            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
					            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
					            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
					            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
					            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
					            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
					            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
					            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
					            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
					            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
					            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
					            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
					            '".$_SESSION['usucpf']."',
					            '".$xml->body->nu_seq_ne."',
					            '".$dados['empid']."',
					            '2 - EFETIVADO'
					            ) RETURNING empid;";
				
				$empid = $db->pegaUm($sql);
				
				$sql = "SELECT eobid, sbaid, empid, eobpercentualemp, eobvalorempenho, eobano, eobstatus
						FROM par.empenhosubacao WHERE empid = {$dados['empid']}";
				$arrEmpSub = $db->carregar($sql);
				$arrEmpSub = $arrEmpSub ? $arrEmpSub : array();
				
				foreach ($arrEmpSub as $v) {
					$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
			    			VALUES ('".$v['sbaid']."', '".$empid."', '".$v['eobpercentualemp']."', '".$v['eobvalorempenho']."', '".$v['eobano']."');";
					$db->executar($sql);
				}
				
				$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    					VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', 'CANCELADO SIGEF');";
				$db->executar($sql);
				
				$boEmpid = $db->pegaUm("select count(es.empid) from par.empenhosubacao es
										inner join par.empenho e on e.empid = es.empid 
									where 
										eobstatus = 'A'
									    and e.empsituacao not ilike('%cancelado%')
										and sbaid in (select sbaid from par.empenhosubacao where empid = {$dados['empid']} and eobstatus = 'A')");
								
				if($nu_seq_ne && $boEmpid == 0){
					$db->executar("update par.subacaodetalhe  set prpid = NULL, ssuid = 2
									 where sbdid in (
										select sbdid from par.subacaodetalhe sd
										inner join  par.empenhosubacao es on es.sbaid = sd.sbaid and es.eobano = sd.sbdano and eobstatus = 'A' 
										inner join par.empenho e on e.empid = es.empid and empstatus = 'A' 
										where empprotocolo = '".$nu_seq_ne."')");
				}
				
				//$db->executar("DELETE FROM par.empenhosubacao WHERE empid='".$dados['empid']."'");				
				//$db->executar("update par.empenhosubacao set eobstatus = 'I' where empid = {$dados['empid']}");
				
				echo "Empenho Cancelado.";					
				$db->commit();
			}
			
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'{$hwpwebservice}',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		} else {
			$errodecancelamento = true;
			$erroCancelamentoEmp =  "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;

			if(count($erros)>0) {
				foreach($erros as $err) {
					$erroCancelamentoEmp .= " \n Erro ao tentar cancelar o empenho. \n";
					$erroCancelamentoEmp .= "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'cancelarEmpenho - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		}


} //FIM IF SE EXISTE NE
  	
if($errodecancelamento){
//------------------- ANULA EMPENHO ----------------------//	

	$sql = "SELECT distinct l.lwsid FROM par.logws l
						inner join par.historicowsprocessopar h ON l.lwsid = h.lwsid
					WHERE
						h.prpid = {$_SESSION['par_var']['prpid']}
        				and h.hwpxmlretorno is null
        				and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessopar where prpid = {$_SESSION['par_var']['prpid']})
						and l.lwstiporequest = '$co_especie_empenho'";
        	$request_id = $db->pegaUm($sql);
        	
        	if( empty($request_id) ){
		        $arrParam = array(
		        		'lwstiporequest' 	=> $co_especie_empenho,
		        		'usucpf' 			=> $_SESSION['usucpf'],
		        );
		        $request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
        	}
	
	    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<request_id>$request_id</request_id>
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;
	
			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
			}
			
			$arrParam = array(
					'prpid' 		=> $_SESSION['par_var']['prpid'],
					'lwsid' 		=> $request_id,
					'hwpxmlenvio' 	=> str_replace( "'", '"', $arqXml),
					'hwpdataenvio' 	=> 'now()',
					'usucpf' 		=> $_SESSION['usucpf']
			);
			$hwpid = logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'insert' );
			
			$arrParam = array(
					'lwsrequestdata' 	=> 'now()',
					'lwsurl' 			=> $urlWS,
					'lwsmetodo' 		=> 'solicitar',
					'lwsid' 			=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
	
			$xmlRetorno = $xml;
			
			$arrParam = array(
					'lwsresponsedata' 	=> 'now()',
					'lwsid' 			=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
			$arrParam = array(
					'hwpid'			=> $hwpid,
					'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno)
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
	
		    $xml = simplexml_load_string( stripslashes($xml));

		    echo "------ ANULAÇÃO DE EMPENHO ------\n\n";
			//echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
			
			$result = (integer) $xml->status->result;
	
			if($result) {
				echo "ANULAÇÃO DE EMPENHO - Sucesso";
				echo $xml->status->result->message->text;
				$hwpwebservice = "ANULAÇÃO DE EMPENHO - Sucesso";
				//$db->executar("UPDATE par.empenho SET empsituacao='CANCELADO', empprotocolo = '{$xml->body->nu_seq_ne}'  WHERE empid='".$dados['empid']."'");
				
				$arrParam = array(
						'lwserro' 	=> false,
						'lwsid' 	=> $request_id
				);
				logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
					
				$arrParam = array(
						'hwpid' 		=> $hwpid,
						'hwpwebservice' => 'ANULAÇÃO DE EMPENHO - Sucesso'
				);
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
				
				$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empvalorempenho, empnumeroprocesso, empcodigoespecie, empcodigopi, empcodigoesfera, empcodigoptres,
				            empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
				            empprogramafnde, empnumerosistema, usucpf, empprotocolo, empidpai, empsituacao)
					    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
					    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
					    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
					    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
					            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
					            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
					            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
					            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
					            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
					            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
					            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
					            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
					            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
					            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
					            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
					            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
					            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
					            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
					            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
					            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
					            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
					            '".$_SESSION['usucpf']."',
					            '".$xml->body->nu_seq_ne."',
					            '".$dados['empid']."',
					            '2 - EFETIVADO'
					            ) RETURNING empid;";
				
				$empid = $db->pegaUm($sql);
				
				$sql = "SELECT eobid, sbaid, empid, eobpercentualemp, eobvalorempenho, eobano, eobstatus 
						FROM par.empenhosubacao WHERE empid = {$dados['empid']}";
				$arrEmpSub = $db->carregar($sql);
				$arrEmpSub = $arrEmpSub ? $arrEmpSub : array();
				
				foreach ($arrEmpSub as $v) {
					$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
			    			VALUES ('".$v['sbaid']."', '".$empid."', '".$v['eobpercentualemp']."', '".$v['eobvalorempenho']."', '".$v['eobano']."');";
					$db->executar($sql);
				}
				
				$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    				VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', 'CANCELADO SIGEF\SIAFI');";				
				$db->executar($sql);
				
				//$db->executar("update par.empenhosubacao set eobstatus = 'I' where empid = {$dados['empid']}");
				$db->commit();			
			} else {
				
				
				echo "\n\n\n";
				echo $erroCancelamentoEmp;
				
				echo "ANULAÇÃO DE EMPENHO  - Erro";
				echo $xml->status->error->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->error->message->text)."\n\n";
				$hwpwebservice = "ANULAÇÃO DE EMPENHO  - Erro";
				
				$arrParam = array(
					'lwserro' 	=> true,
					'lwsid' 	=> $request_id
				);
				logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
				
				$arrParam = array(
						'hwpid' 		=> $hwpid,
						'hwpwebservice' => 'ANULAÇÃO DE EMPENHO - Erro'
				);
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
				
			}
			$db->commit();

		}
	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Cancelar Empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Cancelar Empenho no SIGEF: $erroMSG";
	}
}

function solicitarProcesso($dados) {
	global $db;
	
    $dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prptipoexecucao, muncod, prpbanco, prpagencia, prpdatainclusao, usucpf, prpseqconta, prptipo, seq_conta_corrente, nu_conta_corrente, inuid
    						   FROM par.processopar
    						   WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");
	
    if($dadosse) {
    	$an_processo = date("Y");
    	$nu_processo=$dadosse['prpnumeroprocesso'];
    	$tp_processo=1; // O que vai ser no PAR

    	if($dadosse['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde="03";
	    }else if($dadosse['prptipoexecucao'] == 'T'){ 
	        $co_programa_fnde= "CM";//"ED";
	    }
    	
	    $nu_cnpj_favorecido = pegaCnpj($_SESSION['par_var']['inuid']);
	    /*
	    $nu_cnpj_favorecido = $db->pegaUm("SELECT iue.iuecnpj 
	                                       FROM par.instrumentounidade iu
	                                               inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
	                                       WHERE
	                                               iu.inuid = {$_SESSION['par_var']['inuid']}
	                                           and iue.iuestatus = 'A'
	                                           and iue.iuedefault = true");*/
	   // $nu_cnpj_favorecido = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
	    
    }

    $data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	//$usuario = 'MECTIAGOT';
	//$senha   = 'M3135689';

    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<params>
      	<nu_cnpj>$nu_cnpj_favorecido</nu_cnpj>
      	<nu_processo>$nu_processo</nu_processo>
      	<tp_processo>$tp_processo</tp_processo>
      	<an_processo>$an_processo</an_processo>
      	<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;
//echo $arqXml;
//return true;

	//	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
	//		$urlWS = 'http://172.20.200.116/webservices/corp/integracao/web/dev.php/processo/solicitar';
	//	} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/corp/index.php/processo/solicitar';
	//	}
		
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
				->execute();

		$xmlRetorno = $xml;
		
	    $xml = simplexml_load_string( stripslashes($xml));
		
	    //echo "\n\n------ SOLICITAÇÃO DE PROCESSO ------\n\n";
		//echo "|".$xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
		

		$result = (integer) $xml->status->result;
		
		if(!$result) {
		
			$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO SOLICITAR PROCESSO NO SISTEMA DOCUMENTA: 
		 					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >';
			
			$erros = $xml->status->error->message;
			if(count($erros)>0) {	
				foreach($erros as $err) {	
			 		$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}
				$mensagem .= '</div>
			 			</div>
			 			<br>';
				
				echo $mensagem;
			
			//echo "*** Descrição do erro ***\n\n";
			/*
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
					echo "\n";
				}
			}
			*/
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarProcesso - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
	
			$db->executar($sql);
			$db->commit();

		    return false;
		} else {

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarProcesso - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}

}

function atualizaDadosContaCorrentePar($dados) {
	global $db;
	
    $dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prptipoexecucao, muncod, prpbanco, prpagencia, prpdatainclusao, usucpf, prpseqconta, prptipo, seq_conta_corrente, nu_conta_corrente, inuid
    						   FROM par.processopar
    						   WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");
	
    if($dadosse) {
    	$an_processo = date("Y");
    	$nu_processo=$dadosse['prpnumeroprocesso'];
    	$tp_processo=1; // O que vai ser no PAR

    	if($dadosse['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde="03";
	    }else if($dadosse['prptipoexecucao'] == 'T'){ 
	        $co_programa_fnde= "CM";//"ED";
	    }
    	
	    $nu_cnpj_favorecido = pegaCnpj($_SESSION['par_var']['inuid']);
	    /*
	    $nu_cnpj_favorecido = $db->pegaUm("SELECT iue.iuecnpj 
	                                       FROM par.instrumentounidade iu
	                                               inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
	                                       WHERE
	                                               iu.inuid = {$_SESSION['par_var']['inuid']}
	                                           and iue.iuestatus = 'A'
	                                           and iue.iuedefault = true");*/
	    //$nu_cnpj_favorecido = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
	    
    }

    $data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	$somente_conta_ativa	= 'N';
	$numero_de_linhas		= '200';
	/*$usuario = 'MECTIAGOT';
	$senha   = 'M3135689';*/

    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$data_created}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<nu_identificador>$nu_cnpj_favorecido</nu_identificador>
			<nu_processo>$nu_processo</nu_processo>
			<somente_conta_ativa>$somente_conta_ativa</somente_conta_ativa>
			<numero_de_linhas>$numero_de_linhas</numero_de_linhas>
		</params>
	</body>
</request>
XML;
    
				if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
					//$urlWS = 'http://dev.fnde.gov.br/webservices/sigef/integracao/public/index.php/financeiro/cr';
					$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
					//$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
				} else {
					$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
				}
				
				$xml = Fnde_Webservice_Client::CreateRequest()
						->setURL($urlWS)
						->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoCC') )
						->execute();

				$xmlRetorno = $xml;

				$xml = simplexml_load_string( stripslashes($xml));
		
	    //echo "\n\n------ SOLICITAÇÃO DE PROCESSO ------\n\n";
		//echo "|".$xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
		

		$result = (integer) $xml->status->result;
		
		if(!$result) {
		
			$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF: 
		 					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >';
			
			$erros = $xml->status->error->message;
			if(count($erros)>0) {	
				foreach($erros as $err) {	
			 		$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}
				$mensagem .= '</div>
			 			</div>
			 			<br>';
				
				echo $mensagem;

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'atualizaDadosContaCorrentePar - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
	
			$db->executar($sql);
			$db->commit();

		    return false;
		} else {
			$obContaCorrenteWS = $xml->body->row->children();
			
			$seq_solic_cr 		= !empty($obContaCorrenteWS->seq_solic_cr) ? "'".(int)$obContaCorrenteWS->seq_solic_cr."'" : 'null';
			$seq_conta 			= !empty($obContaCorrenteWS->seq_conta) ? "'".(int)$obContaCorrenteWS->seq_conta."'" : 'null';
			$dt_movimento 		= !empty($obContaCorrenteWS->dt_movimento) ? "'".(string)$obContaCorrenteWS->dt_movimento."'" : 'null';
			$nu_banco 			= !empty($obContaCorrenteWS->nu_banco) ? "'".(string)$obContaCorrenteWS->nu_banco."'" : 'null';
			$nu_agencia 		= !empty($obContaCorrenteWS->nu_agencia) ? "'".(string)$obContaCorrenteWS->nu_agencia."'" : 'null';
			$nu_conta_corrente	= !empty($obContaCorrenteWS->nu_conta_corrente) ? "'".(string)$obContaCorrenteWS->nu_conta_corrente."'" : 'null';
			$fase_solicitacao	= !empty($obContaCorrenteWS->fase_solicitacao) ? "'".(string)$obContaCorrenteWS->fase_solicitacao."'" : 'null';
			$co_situacao_conta	= !empty($obContaCorrenteWS->co_situacao_conta) ? "'".(string)$obContaCorrenteWS->co_situacao_conta."'" : 'null';
			$situacao_conta 	= !empty($obContaCorrenteWS->situacao_conta) ? "'".(string)$obContaCorrenteWS->situacao_conta."'" : 'null';
			$nu_processo 		= !empty($obContaCorrenteWS->nu_processo) ? "'".(string)$obContaCorrenteWS->nu_processo."'" : 'null';
			$nu_identificador 	= !empty($obContaCorrenteWS->nu_identificador) ? "'".(string)$obContaCorrenteWS->nu_identificador."'" : 'null';
			$ds_razao_social 	= !empty($obContaCorrenteWS->ds_razao_social) ? "'".(string)$obContaCorrenteWS->ds_razao_social."'" : 'null';
			$ds_problema		= "'-'";
			$rnum 				= (int)		$obContaCorrenteWS->rnum;
			$status 			= (string)	$obContaCorrenteWS->status;
			$co_status			= substr( $status, 0, 1 );
			
			if( trim($co_status) != 0 ){
				$sql = "UPDATE par.processopar SET
							prpbanco = $nu_banco,
						  	prpagencia = $nu_agencia,
						  	prpseqconta = $seq_solic_cr,
						  	seq_conta_corrente = $seq_conta,
						  	nu_conta_corrente = $nu_conta_corrente,
						  	dt_movimento = $dt_movimento,
						  	fase_solicitacao = $fase_solicitacao,
						  	co_situacao_conta = $co_situacao_conta,
						  	situacao_conta = $situacao_conta,
						  	nu_identificador = $nu_identificador,
						  	ds_razao_social = $ds_razao_social						 
						WHERE 
						  	prpid = {$_SESSION['par_var']['prpid']}";
				$db->executar($sql);
				$db->commit();
				
				$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'atualizaDadosContaCorrentePar - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

				$db->executar($sql);
				$db->commit();
	
				return true;
				
			} else {
				$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF: 
		 					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >';
			
					
				$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $status);
				$mensagem .= '</div>
				 			</div>
				 			<br>';
					
				echo $mensagem;
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'atualizaDadosContaCorrentePar - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
				return false;
			}
		}

}



function consultarContaCorrente($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

        $prpseqconta = $db->pegaUm("SELECT prpseqconta FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");
        /*
		if(!$prpseqconta) {
			$existeAndamentoConta = consultarAndamentoContaCorrente($dados);
			if(!$existeAndamentoConta){
			 // RETORNO FALSE - SE NÃO EXISTE CONTA  EM ANDAMENTO PARA SER ABERTA SOLICITA CONTA.
				$r = solicitarContaCorrente($dados);
				if($r){
					return "cc_criado_sucesso";
		       	}else{
		       		return false;
		       	}
			}
	     }*/
		
    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <seq_solic_cr>$prpseqconta</seq_solic_cr>
		</params>
	</body>
</request>
XML;

	//	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
	//		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
	//	} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	//	}

		if($prpseqconta) {

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
					->execute();

			$xmlRetorno = $xml;

		    $xml = simplexml_load_string( stripslashes($xml));

		    if($xml->body->row->seq_conta) {
		    	$db->executar("UPDATE par.processopar SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE prpseqconta='".$prpseqconta."'");
		    	$db->commit();
		    }

		    echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					CONSULTA DE CONTA CORRENTE EFETUADA COM SUCESSO: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Status da conta: '.iconv("UTF-8", "ISO-8859-1", $xml->body->row->status).'<br>
						Fase solicitação: '.(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-').'
					</div>
				</div>
			 	<br>
				';
		/*
			echo "------ CONSULTA DE CONTA CORRENTE ------\n\n";
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Detalhes da consulta ***\n\n";
			echo "* Data movimento:".(($xml->body->row->dt_movimento)?$xml->body->row->dt_movimento:'-')."\n";
			echo "* Fase solicitação:".(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-')."\n";
			echo "* Entidade:".(($xml->body->row->ds_razao_social)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social):'-')."(".(($xml->body->row->nu_identificador)?$xml->body->row->nu_identificador:'-').")\n\n";
		*/
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'consultarContaCorrente',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    $result = (integer) $xml->status->result;

		    if($result) {
		    	return false;
		    } else {
		    	return true;
		    }

		} else {
			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO CONSULTAR CONTA CORRENTE: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.
					</div>
				</div>
			 	<br>
				';
			
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		//echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
		
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO CONSULTAR CONTA CORRENTE: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro-WS Consultar Conta Corrente no SIGEF: '.$erroMSG.'
					</div>
				</div>
			 	<br>
				';

	}
}

// colocar tipo de subação e colocar um filtro tb.
function cabecalhoSolicitacaoEmpenhoPar()  {
	global $db;
	$arrDados = $db->pegaLinha("SELECT 
									   m.muncod,
									   CASE WHEN e.estuf IS NOT NULL THEN e.estuf ELSE m.estuf END as estuf,
									   CASE WHEN e.estuf IS NOT NULL THEN e.estdescricao ELSE m.mundescricao END as descricao,
									   p.prpnumeroprocesso
									--   CASE WHEN p.prptipo='P' THEN 'Padrão' ELSE 'Ver' END as tipoobra,
									--   p.prptipo
								FROM par.processopar p
								INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
								LEFT JOIN territorios.municipio m ON m.muncod = p.muncod
								LEFT JOIN territorios.estado e ON e.estuf = iu.estuf
								WHERE p.prpid='{$_SESSION['par_var']['prpid']}'
								and p.prpstatus = 'A'");
	
	echo "<table border=0 cellpadding=3 cellspacing=0 class=listagem width=98% align=center>";
	echo "<tr>";
	echo "<td class=SubTituloDireita width='30%'>UF:</td>";
	echo "<td>".$arrDados['estuf']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Local:</td>";
	echo "<td>".$arrDados['descricao']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Nº processo:</td>";
	echo "<td>".formatarProcesso($arrDados['prpnumeroprocesso'])."</td>";
	echo "</tr>";
	echo "</table>";
}


function solicitarContaCorrente($dados) {
	global $db;
	
	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

        $dadoscc = $db->pegaLinha("SELECT prpnumeroprocesso,prptipoexecucao, prpbanco, prpagencia, muncod, prptipo, inuid FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");
		
        if($dadoscc) {
	        // numero do processo (No desenvolvimento é fixo)
        //	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
        //	   $_SESSION['baselogin'] == "simec_espelho_producao" ){
        	   	//$nu_processo='23034655466200900';
        //	   	$nu_processo=$dadoscc['prpnumeroprocesso'];//234000005642011
        //	} else {
	        	$nu_processo=$dadoscc['prpnumeroprocesso'];
        //	}

	        // constante=001
	        $nu_banco=$dadoscc['prpbanco'];
	        // esperando envio
	        $nu_agencia=trim((int)$dadoscc['prpagencia']);
        }
        
		// CNPJ da prefeitura
		$nu_identificador = pegaCnpj($_SESSION['par_var']['inuid']);
		/*
		$nu_identificador = $db->pegaUm("SELECT iue.iuecnpj 
	                                       FROM par.instrumentounidade iu
	                                               inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
	                                       WHERE
	                                               iu.inuid = {$_SESSION['par_var']['inuid']}
	                                           and iue.iuestatus = 'A'
	                                           and iue.iuedefault = true");*/
        //$nu_identificador = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);

  		// constante=1
        $tp_identificador="1";

        // constante=nulo
        $nu_conta_corrente=null;
        // constante=01
        $tp_solicitacao="01";
        // constante=0032
        $motivo_solicitacao="0032";
        // constante=nulo
        $convenio_bb=null;
        // constante=N
        $tp_conta="N";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
        
		if($dadoscc['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde="03";
	   		$nu_sistema="2";
	    }else if($dadoscc['prptipoexecucao'] == 'T'){ 
	        $co_programa_fnde="CM"; //"ED";
	        $nu_sistema="7";
	    }
       



    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_identificador>$nu_identificador</nu_identificador>
        <tp_identificador>$tp_identificador</tp_identificador>
        <nu_processo>$nu_processo</nu_processo>
        <nu_banco>$nu_banco</nu_banco>
        <nu_agencia>$nu_agencia</nu_agencia>
        <nu_conta_corrente>$nu_conta_corrente</nu_conta_corrente>
        <tp_solicitacao>$tp_solicitacao</tp_solicitacao>
        <motivo_solicitacao>$motivo_solicitacao</motivo_solicitacao>
        <convenio_bb>$convenio_bb</convenio_bb>
        <tp_conta>$tp_conta</tp_conta>
        <nu_sistema>$nu_sistema</nu_sistema>
        <co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;

	//	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
	//	   $_SESSION['baselogin'] == "simec_espelho_producao" ){
	//		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
	//	} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	//	}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

	    /*
	    echo "------ SOLICITAÇÃO DE CONTA CORRENTE ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
		*/

		$result = (integer) $xml->status->result;
		if(!$result) {
			
			 $mensagem =  
			 	'<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO SOLICITAR ABERTURA DE CONTA CORRENTE:
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Descrição do erro: ';
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					//echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
					$mensagem .= ' '.iconv("UTF-8", "ISO-8859-1", $err->text).' ';
				}
			}
			 $mensagem .= '
					</div>
				</div>
			 	<br>
				';
			 echo $mensagem;
			
			
			//echo "*** Descrição do erro ***\n\n";
			

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarContaCorrente - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    return false;
		} else {
			
		    $db->executar("UPDATE par.processopar SET prpseqconta='".$xml->body->seq_solic_cr."', seq_conta_corrente='".$xml->body->nu_seq_conta."' WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarContaCorrente - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO SOLICITAR A CONTA CORRENTE: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.
					</div>
				</div>
			 	<br>
				';
			
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		//echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
		
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO SOLICITAR A CONTA CORRENTE: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						'.$erroMSG.'
					</div>
				</div>
			 	<br>
				';

	}
}



function listaSubacao($dados) {
	global $db;
	
	// LISTA DE SUBAÇÕES 100% EMPENHADAS
	/* Alterado dia 31/10/2014 por Eduardo Dunice Neto atendendo a demanda 1263
	 * Estava utilizando a forma antiga de buscar os valores de emepnho. Troquei pela view de empenho subação.
	 * */
	$sql = "SELECT
				botao,
				numeroempenho,
				descricaosubacao,
				cast(valorsubacao as numeric(20,2)) as valorsubacao,
				--CAST((valorempenhado*100/valorsubacao) as NUMERIC(30,2))||'&nbsp;' as percentualempenhado,
				case when valorsubacao > 0 then
					CAST((valorempenhado*100/valorsubacao) as NUMERIC(30,2))||'&nbsp;'
				else '0&nbsp;' end as percentualempenhado,
				valorempenhado,
				ano
			FROM(
				SELECT DISTINCT
					'<a href=\"javascript:listarSubacao(\'' || s.sbaid || '\', \'' || sd.sbdano || '\', \'' || iu.inuid || '\')\" >
       				<img src=\"../imagens/consultar.gif\" title=\"Visualizar Subação\" border=0 ></a>' as botao, 
					e.empnumero as numeroempenho, 
					s.sbadsc as descricaosubacao, 
					--(SELECT par.recuperaValorValidadosSubacaoPorAno(s.sbaid , sd.sbdano)::numeric(20,2)) AS valorsubacao,
					(SELECT coalesce(par.recuperaValorValidadosSubacaoPorAno(sd.sbaid, sd.sbdano), 0) - (coalesce(sd.sbdrepassevlrrafaprovado,0) + coalesce(sd.sbdrepassevlrcomplementaraprovado,0))) AS valorsubacao,
					ves.saldo as valorempenhado,
					sd.sbdano || '&nbsp;' as ano
				FROM par.instrumentounidade 	iu
					INNER JOIN par.pontuacao 				po  ON po.inuid = iu.inuid
					INNER JOIN par.acao 					a   ON a.ptoid  = po.ptoid
					INNER JOIN par.subacao					s   ON s.aciid  = a.aciid 
					INNER JOIN par.processopar 				p   ON p.inuid 	= iu.inuid and p.prpstatus = 'A'
					INNER JOIN par.subacaodetalhe 			sd  ON sd.sbaid = s.sbaid 
					INNER JOIN par.empenhosubacao			es  ON es.sbaid = s.sbaid AND es.eobano = sd.sbdano and es.eobstatus = 'A'
					INNER JOIN par.empenho 					e   ON e.empid 	= es.empid AND e.empcodigoespecie NOT IN ('03', '13', '02', '04') and e.empnumeroprocesso = p.prpnumeroprocesso
					INNER JOIN par.v_dadosempenhosubacao 	ves ON ves.empid = e.empid AND ves.sbaid = es.sbaid and ves.sbdano = es.eobano
					LEFT  JOIN par.empenhopregaoitemperdido epi ON epi.prpid = p.prpid AND epi.sbdid = sd.sbdid
				WHERE 
					true in (SELECT DISTINCT
						CASE WHEN prptipoexecucao = 'C' THEN
							sum(eobvalorempenhomenoscancelado) >= CAST((vrlsubacao*99/100) as numeric(20,2))
						ELSE 
							sum(eobvalorempenhomenoscancelado) >= vrlsubacao
						END as d
					FROM(
						SELECT DISTINCT
							ems.sbaid,
							ems.eobano,
							prptipoexecucao,
							--cast((SELECT par.recuperaValorValidadosSubacaoPorAno(ems.sbaid , ems.eobano) ) as numeric(20,2)) as vrlsubacao,
							(SELECT coalesce(par.recuperaValorValidadosSubacaoPorAno(sd.sbaid, sd.sbdano), 0) - (coalesce(sd.sbdrepassevlrrafaprovado,0) + coalesce(sd.sbdrepassevlrcomplementaraprovado,0))) AS vrlsubacao,
							ves.saldo as eobvalorempenhomenoscancelado
						FROM par.processopar pr
							INNER JOIN par.empenho 					e   ON e.empnumeroprocesso = pr.prpnumeroprocesso AND e.empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A' 
							INNER JOIN par.empenhosubacao 			ems ON ems.empid = e.empid AND ems.eobstatus = 'A'
							inner join par.subacaodetalhe 			sd on sd.sbaid = ems.sbaid and sd.sbdano = ems.eobano
							INNER JOIN par.v_dadosempenhosubacao 	ves ON ves.empid = e.empid AND ves.sbaid = ems.sbaid AND ves.sbdano = ems.eobano
						WHERE  pr.prpid =  {$_SESSION['par_var']['prpid']} AND pr.inuid = {$_SESSION['par_var']['inuid']} --ems.sbaid = es.sbaid
						) as foo
						group by sbaid, prptipoexecucao, vrlsubacao
					)
					AND p.prpid = {$_SESSION['par_var']['prpid']}
					AND iu.inuid = {$_SESSION['par_var']['inuid']}
			) as foo
			order by descricaosubacao";
 	//ver(simec_htmlentities($sql), d);
	
	/*
	 * SELECT DISTINCT CASE WHEN pr.prptipoexecucao = 'C'
						THEN
							CASE WHEN emr.reevlr IS NOT NULL THEN 
								sum(ems.eobvalorempenho+emr.reevlr) = (SELECT par.recuperaValorValidadosSubacaoPorAno(ems.sbaid , ems.eobano )*99/100::numeric(20,2))
							ELSE
								sum(ems.eobvalorempenho) = (SELECT par.recuperaValorValidadosSubacaoPorAno(ems.sbaid , ems.eobano )*99/100::numeric(20,2))
							END
						ELSE 
							CASE WHEN emr.reevlr IS NOT NULL THEN 
								sum(ems.eobvalorempenho+emr.reevlr) = (SELECT par.recuperaValorValidadosSubacaoPorAno(ems.sbaid , ems.eobano)::numeric(20,2))
							ELSE
								sum(ems.eobvalorempenho) = (SELECT par.recuperaValorValidadosSubacaoPorAno(ems.sbaid , ems.eobano)::numeric(20,2))
							END
						END as d
					FROM par.processopar pr
					INNER JOIN par.empenho e on e.empnumeroprocesso = pr.prpnumeroprocesso and e.empcodigoespecie not in ('03', '13', '02', '04')
					LEFT JOIN par.empenhoreforco emr ON emr.empid = e.empid
					INNER JOIN par.empenhosubacao ems on ems.empid = e.empid and ems.eobstatus = 'A'
					WHERE  pr.prpid =  {$_SESSION['par_var']['prpid']} AND es.sbaid = ems.sbaid and pr.prpstatus = 'A'
					GROUP BY prptipoexecucao, ems.eobano, ems.sbaid, emr.reevlr*/

	echo '
		<table id="listaSub100Empenhada" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<thead>
				<tr>
					<td>Lista de Subações 100% empenhadas do tipo Termo e 99% do tipo Convênio</td>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>
	';			
		$cabecalho = array("&nbsp;","Nº do Empenho","Subação", "Valor da Subação", "% Empenhado", "Valor Empenhado", "Ano" );
		$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');

	echo '
					</td>
				</tr>
			</tbody>
		</table>
	';
		
	$processo = $db->pegaUm( "SELECT prpnumeroprocesso FROM par.processopar WHERE  prpstatus = 'A' and prpid = ".$_SESSION['par_var']['prpid'] );
	
	// LISTA DE SUBAÇÕES A SEREM EMPENHADAS
	
	$_SESSION['par_var']['esfera'] = $db->pegaUm("SELECT CASE WHEN itrid = 1 THEN 'estadual' ELSE 'municipal' END as esfera FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
	$tpcid = $_SESSION['par_var']['esfera'] == 'estadual' ? 1 : 3;
	if($_SESSION['par_var']['inuid'] == 1){
		$tpcid = 1;
	}
	

	$sql = "
	SELECT DISTINCT
	'<input type=checkbox name=chk[] onclick=marcarChk(this); id=chk_'||dados.sbdid||' value='||dados.sbdid||'>' as chk,
	'<a href=\"javascript:listarSubacao(\'' || dados.sbaid || '\', \'' || dados.sbdano || '\', \'' || dados.inuid || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar Subação\" border=\"0\"></a>' as consulta,
	dados.subacao,
	 dados.valorsubacao as valorsubacao,
	--(SUM(dados.valorempenhado)*100/dados.valorsubacao) as percentualempenhado,
	'<center>'||case when dados.valorsubacao > 0 then cast((SUM(dados.valorempenhado)*100/dados.valorsubacao) as numeric(20,2)) else 0 end||'&nbsp;</center>' as percentualempenhado,
	SUM(dados.valorempenhado) as valorempenhado,
	'<input type=text id=id_'||dados.sbdid||' value=\"'||CASE WHEN dados.prptipoexecucao = 'C' 
		THEN
			--(99 - (SUM(dados.valorempenhado)*100/dados.valorsubacao))
			case when dados.valorsubacao > 0 then (99 - (SUM(dados.valorempenhado)*100/dados.valorsubacao)) else 0 end 
		ELSE
			--(100 - (SUM(dados.valorempenhado)*100/dados.valorsubacao))
			case when dados.valorsubacao > 0 then (100 - (SUM(dados.valorempenhado)*100/dados.valorsubacao)) else 0 end 
		END||'\" name=name_'||dados.sbdid||' size=6 onKeyUp=\"calculaEmpenho(this);\" onBlur=\"verificaPreenchimentoPorcentagem(this) \" class=\"disabled\" readonly=readonly onfocus=\"this.select();\">
						<input type=hidden id=vlr_'||dados.sbdid||' name=vlr_'||dados.sbdid||' value=\"'||CASE WHEN dados.prptipoexecucao = 'C' 
		THEN
			--(99 - (SUM(dados.valorempenhado)*100/dados.valorsubacao))
			case when dados.valorsubacao > 0 then (99 - (SUM(dados.valorempenhado)*100/dados.valorsubacao)) else 0 end 
		ELSE
			--(100 - (SUM(dados.valorempenhado)*100/dados.valorsubacao))
			case when dados.valorsubacao > 0 then (100 - (SUM(dados.valorempenhado)*100/dados.valorsubacao)) else 0 end 
		END||'\">
	<input type=hidden id=anosubacao_'||dados.sbdid||' name=anosubacao_'||dados.sbdid||' value='||dados.sbdano||'>' as percentualaempenhar,
	'<input type=text id=id_vlr_'||dados.sbdid||' value='||(dados.valorsubacao - SUM(dados.valorempenhado))::numeric(20,2)||' name=name_vlr_'||dados.sbdid||' size=15 onBlur=\"this.value=mascaraglobal(\'[.###],##\',this.value); calculaEmpenho(this); verificaPreenchimentoPorcentagem(this);\" onKeyUp=\"this.value=mascaraglobal(\'[.###],##\',this.value); calculaEmpenho(this);\" class=\"disabled\" readonly=readonly onfocus=\"this.select();\">
	<input type=\"hidden\" name=\"vlrsubacao['||dados.sbdid||']\" id=\"vlrsubacao\" value=\"'||dados.valorsubacao||'\">' as valoraempenhar,
	dados.planointerno,
	dados.ptres,
	dados.sbdano || '&nbsp;' as ano,
	dados.reforco,
	dados.vrlrafaprovado,
	dados.vrlcomplementaraprovado,
	'<input type=hidden onBlur=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" onKeyUp=\"this.value=mascaraglobal(\'[.###],##\',this.value);\"  id=vlrTotal_'||dados.sbdid||' name=vlrTotal_'||dados.sbdid||' value='|| (dados.valorsubacao - SUM(dados.valorempenhado))::numeric(20,2) ||'>' as valoraempenharT
FROM (
	SELECT DISTINCT
		sd.sbdid,
		sd.sbaid,
		iu.inuid,
		s.sbadsc as subacao,
		--(SELECT par.recuperaValorValidadosSubacaoPorAno(sd.sbaid, sd.sbdano)) as valorsubacao,
		(SELECT coalesce(par.recuperaValorValidadosSubacaoPorAno(sd.sbaid, sd.sbdano), 0) - (coalesce(sd.sbdrepassevlrrafaprovado,0) + coalesce(sd.sbdrepassevlrcomplementaraprovado,0))) as valorsubacao,
		CASE WHEN emr.vrlreforco IS NOT NULL THEN 
			(coalesce(es.eobvalorempenho,0) - coalesce(emc.vlrempenhocancelado, 0))+coalesce(emr.vrlreforco,0)
		ELSE
			(coalesce(es.eobvalorempenho,0) - coalesce(emc.vlrempenhocancelado, 0))
		END as valorempenhado,
		p.prptipoexecucao,
		sd.sbdplanointerno as planointerno,
		sd.sbdptres||'&nbsp;' as ptres,
		sd.sbdano,
		CASE WHEN sd.sbdreforco = TRUE THEN '<input type=\"button\" name=\"reforco\" id=\"reforco\" value=\"Solicitar Reforço\" onclick=reforcoSub(\'' || sd.sbdid || '\')>' ELSE '' END as reforco,
		coalesce(sd.sbdrepassevlrrafaprovado,0) as vrlrafaprovado,
		coalesce(sd.sbdrepassevlrcomplementaraprovado,0) as vrlcomplementaraprovado
	FROM 
		par.instrumentounidade 	iu
	INNER JOIN par.pontuacao 	po on po.inuid   = iu.inuid AND ptostatus <> 'I'
	INNER JOIN par.acao 		a  on a.ptoid   = po.ptoid
	INNER JOIN par.subacao		s  on s.aciid   = a.aciid AND sbastatus = 'A'
	INNER JOIN par.processopar 	p  ON p.inuid = iu.inuid
	INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid --AND sd.ssuid IN (2, 12, 13, 18, 17)
	LEFT JOIN par.empenhosubacao 	es 
		INNER JOIN par.empenho 		e  ON e.empid = es.empid AND e.empnumeroprocesso = '".$processo."'  and eobstatus = 'A' and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
	ON es.sbaid = sd.sbaid AND es.eobano = sd.sbdano
	left join (
		select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.sbaid, eb.eobano from par.empenhosubacao eb
			inner join par.empenho e1 on e1.empid = eb.empid
		where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null and empstatus = 'A' and eobstatus = 'A'
		group by e1.empidpai, eb.sbaid, eb.eobano
	) as emc on emc.empidpai = es.empid and emc.sbaid = es.sbaid and emc.eobano = sd.sbdano
	left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
			from par.empenho
			where empcodigoespecie in ('02') and empstatus = 'A'
			group by 
				empnumeroprocesso,
				empcodigoespecie,
				empidpai) as emr on emr.empidpai = es.empid
	inner join par.processoparcomposicao ppc on ppc.prpid = p.prpid and sd.sbdid = ppc.sbdid and ppc.ppcstatus = 'A'
	WHERE
		p.prpid = {$_SESSION['par_var']['prpid']}
		and p.prpstatus = 'A'
		AND iu.inuid = {$_SESSION['par_var']['inuid']}
		AND s.frmid IN (13,6, 14,15, 16)
		AND sd.sbdid NOT IN ( SELECT sbdid FROM (
												SELECT 
													CASE WHEN prptipoexecucao = 'C' THEN
														eobvalorempenhomenoscancelado >= cast((vrlsubacao*99/100) as numeric(20,2))
													ELSE 
														eobvalorempenhomenoscancelado >= vrlsubacao
													END as teste, sbdid
													 FROM ( 
													 SELECT 
														 ems.sbaid,
														 ems.eobano,
														 sd.sbdid,
														 prptipoexecucao,
														 --cast((SELECT par.recuperaValorValidadosSubacaoPorAno(ems.sbaid , ems.eobano) ) as numeric(20,2)) as vrlsubacao,
														 (SELECT coalesce(par.recuperaValorValidadosSubacaoPorAno(sd.sbaid, sd.sbdano), 0) - (coalesce(sd.sbdrepassevlrrafaprovado,0) + coalesce(sd.sbdrepassevlrcomplementaraprovado,0))) AS vrlsubacao,
														 case when sum(emc.vlrempenhocancelado) > 0 then
																		 (sum(ems.eobvalorempenho) + coalesce( sum(emr.vrlreforco), 0) )  - coalesce(sum(emc.vlrempenhocancelado),0) 
														 else (sum(ems.eobvalorempenho) + coalesce( sum(emr.vrlreforco), 0) ) end as eobvalorempenhomenoscancelado
													 FROM par.processopar pr
														 INNER JOIN par.empenho e on e.empnumeroprocesso = pr.prpnumeroprocesso and e.empcodigoespecie not in ( '03', '13', '02') and empstatus = 'A'
														 INNER JOIN par.empenhosubacao ems on ems.empid = e.empid and ems.eobstatus = 'A'
														 INNER JOIN par.subacaodetalhe sd on sd.sbaid = ems.sbaid AND sbdano = eobano
														 left join (
																		 select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.sbaid 
																		 from par.empenhosubacao eb
																						inner join par.empenho e1 on e1.empid = eb.empid
																		 where e1.empcodigoespecie in ( '03', '13') and empidpai is not null and empstatus = 'A' and eobstatus = 'A'
																		 group by e1.empidpai, eb.sbaid
														 ) as emc on emc.empidpai = e.empid and emc.sbaid = es.sbaid
														 left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
																	from par.empenho
																	where empcodigoespecie in ('02') and empstatus = 'A'
																	group by 
																		empnumeroprocesso,
																		empcodigoespecie,
																		empidpai) as emr on emr.empidpai = e.empid
													 WHERE   pr.prpstatus = 'A' and pr.inuid = {$_SESSION['par_var']['inuid']} AND pr.prpid = {$_SESSION['par_var']['prpid']}
													 GROUP BY
																	 sd.sbdid, eobano, ems.sbaid, prptipoexecucao, sd.sbaid, sd.sbdano, sd.sbdrepassevlrrafaprovado, sd.sbdrepassevlrcomplementaraprovado
													 ) AS foo
		) as foo2
		
		WHERE 
			foo2.sbdid IS NOT NULL AND foo2.teste = true
		) 
	) as dados
GROUP BY
	dados.inuid,
	dados.sbaid,
	dados.sbdid,
	dados.subacao,
	dados.valorsubacao,
	dados.prptipoexecucao,
	dados.planointerno,
	dados.ptres,
	dados.sbdano,
	dados.reforco,
	dados.vrlrafaprovado,
	dados.vrlcomplementaraprovado";
//ver( simec_htmlentities($sql),d );			
	echo '
	
		<table id="listaSubASeremEmpenhada" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<thead>
				<tr>
					<td>Lista de Subações a serem Empenhadas</td>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>
	';
//		$cabecalho = array("&nbsp;","Nº do Empenho","Subação","Valor da Subação", "% Empenhado", "Valor Empenhado", "% a Empenhar", "Valor a Empenhar", "Plano Interno", "PTRES", "Ano" );
		$cabecalho = array("&nbsp;","&nbsp;","Subação","Valor da Subação", "% Empenhado", "Valor Empenhado", "% a Empenhar", "Valor a Empenhar", "Plano Interno", "PTRES", "Ano", "Reforço", "Valor RAF", "Valor Complementar" );
		$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');
	
	echo '			</td>
				</tr>
			</tbody>
		</table>
	';
}

/* function listaEmpenhoProcesso($dados) {
	global $db;

		if($dados['empnumeroprocesso']) {
			$where[] = "empnumeroprocesso='".$dados['empnumeroprocesso']."'";
		} elseif($_SESSION['par_var']['prpid']) {
			$where[] = "prpid='".$_SESSION['par_var']['prpid']."'";
		}
		$_SESSION['par_var']['esfera'] = $db->pegaUm("SELECT CASE WHEN itrid = 1 THEN 'estadual' ELSE 'municipal' END as esfera FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
		if($_SESSION['par_var']['esfera']=='estadual') {
		//	$where[] = "funid = 6";
		} else {
		//	$where[] = "funid = 1";
		}

		
		$perfil = pegaPerfilGeral();
		//regras de acesso passada por Thiago em 24/05/2012
		if(	in_array(PAR_PERFIL_EMPENHADOR, $perfil) ||
			in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
			in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)
		){
			$acaoConsultar = "'<center><img src=../imagens/refresh2.gif style=cursor:pointer; title=\"Consultar Empenho\" onclick=consultarEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\');></center>'";
			$acaoCancelarEmpenho = "'<center><img src=../imagens/excluir.gif align=absmiddle style=cursor:pointer; title=\"Cancelar Empenho\" onclick=\"cancelarEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\', ''cancelar'');\"></center>'";
			$acaoAnularEmpenho = "'<center><img src=../imagens/excluir.gif align=absmiddle style=cursor:pointer; title=\"Anular Empenho\" onclick=\"cancelarEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\', ''anular'');\"></center>'";
			$acaoReduzirEmpenho = "'<center><img src=../imagens/restricao_ico.png align=absmiddle style=cursor:pointer; title=\"Reduzir Empenho\" onclick=\"reduzirEmpenho('||empid||',\'' || trim(empnumeroprocesso) || '\', ''reduzir'');\"></center>'";
			$acaoCancelamento = "'<img src=../imagens/icone_lupa.png align=absmiddle style=cursor:pointer; title=\"Historico de Cancelamento\" onclick=\"historicoCancelamento('||empid||');\">'";
		}else{
			$acaoConsultar = "''";
			$acaoCancelarEmpenho = "''";
			$acaoAnularEmpenho = "''";
			$acaoReduzirEmpenho = "''";
			$acaoCancelamento = "''";
		}	
		
		$sql = "select 
					DISTINCT mais ||
						CASE WHEN situacao = 'CANCELADO' 
					   		THEN $acaoCancelamento 
					   		ELSE '' END as acao,
					   CASE WHEN empsituacao != 'CANCELADO' THEN $acaoConsultar ELSE '&nbsp;' END as acao_consultar,
					   CASE WHEN empvalorempenho > vrlcancelado 
					   		THEN $acaoReduzirEmpenho 
					   		ELSE '' 
					   END as acao_reduzir,
					   CASE WHEN empvalorempenho > vrlcancelado 
					   		THEN $acaoAnularEmpenho 
					   		ELSE '' 
					   END as acao_cancelar,
					  	empcnpj, empprotocolo, empnumero, empvalorempenho, 
					  	vrlcancelado, 
					  	usunome, 
					   empsituacao 
				from(
					SELECT DISTINCT '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoEmpenho(\''||e.empid||'\', this);\">' as mais,
						   e.empid, 
						   e.empnumeroprocesso, 
						   e.empcnpj, 
						   e.empprotocolo, 
						   e.empnumero, 
						   e.empvalorempenho, 
						   u.usunome, 
						   e.empsituacao,
						   case when (select count(empid) from par.empenho where empidpai = e.empid and empcodigoespecie in ('03', '13', '04')) > 0 then 'CANCELADO' else e.empsituacao end as situacao,
						   (select coalesce(sum(empvalorempenho), 0.00) from par.empenho where empidpai = e.empid and empcodigoespecie in ('03', '13', '04')) as vrlcancelado
					FROM par.empenho e
						LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
					WHERE
						e.empcodigoespecie = '01'
					".(($where)?" and  ".implode(" AND ", $where):"")."
				) as foo";
//ver($sql);
	$cabecalho = array("Histórico", "Atualizar", "Reduzir", "Cancelar", "CNPJ","Nº protocolo","Nº empenho","Valor empenho(R$)", "Valor Cancelado(R$)", "Usuário criação","Situação empenho");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%','S', true, '', false, true);
} */


function consultarEmpenho($dados) {
	global $db;

	try {
		
		$data_created = date("c");

		/* if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$nu_seq_ne = "73289";
		} else { */
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];

		    $dadosemp = $db->pegaLinha("SELECT * FROM par.empenho WHERE empid='".$dados['empid']."'");
		    
		    $prpid = $db->pegaUm("select prpid from par.processopar where prpnumeroprocesso = '{$dadosemp['empnumeroprocesso']}'");

	        if($dadosemp) {
	        	$nu_seq_ne = $dadosemp['empprotocolo'];
	        }
		//}

    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;

		/*if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {*/
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		//}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));
		$result = (integer) $xml->status->result;
	    
		if($result){
			$hwpwebservice="consultarEmpenho - Sucesso";
			
			/* if( (string) $xml->body->row->co_especie_empenho == '03' ){
				$situacaoEmpenho = 'CANCELADO';
			} else { */
				$situacaoEmpenho = iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento);
			//}
			
			echo "------ CONSULTA DE EMPENHO ------\n\n";
			echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
			
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Detalhes da consulta ***\n\n";
			echo "* Nº processo : ".(($xml->body->row->processo)?$xml->body->row->processo:"-")."\n";
			echo "* CNPJ : ".$xml->body->row->nu_cnpj."\n";
			echo "* Valor(R$) : ".number_format((string)$xml->body->row->valor_ne,2,",",".")."\n";
			echo "* Data : ".$xml->body->row->data_documento."\n";
			echo "* Nº documento : ".((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-")."\n";
			echo "* Valor empenhado(R$) : ".((strlen($xml->body->row->valor_total_empenhado))?$xml->body->row->valor_total_empenhado:"-")."\n";
			echo "* Saldo pagamento(R$) : ".((strlen($xml->body->row->valor_saldo_pagamento))?$xml->body->row->valor_saldo_pagamento:"-")."\n";
			echo "* Situação : ".$situacaoEmpenho."\n";
			echo "* Espécie : ".$xml->body->row->co_especie_empenho.' - '.$situacaoEmpenho."\n\n";
					
			$set = array();
			if( $xml->body->row->nu_cnpj ) 						$set[] = "empcnpj = '".$xml->body->row->nu_cnpj."'";
			if( $xml->body->row->processo ) 					$set[] = "empnumeroprocesso = '".$xml->body->row->processo."'";
			if( $xml->body->row->nu_seq_ne ) 					$set[] = "empprotocolo = '".$xml->body->row->nu_seq_ne."'";
			if( $xml->body->row->co_especie_empenho ) 			$set[] = "empcodigoespecie = '".$xml->body->row->co_especie_empenho."'";
			if( $xml->body->row->situacao_documento ) 			$set[] = "empsituacao = '".$situacaoEmpenho."'";
			if( trim($xml->body->row->numero_documento) ){
				$empnumerooriginal 	= substr($xml->body->row->numero_documento, 6);
				$empanooriginal 	= substr($xml->body->row->numero_documento, 0, 4);
				
				$set[] = "empnumero = '".$xml->body->row->numero_documento."'";
				$set[] = "empnumerooriginal = ".($empnumerooriginal ? "'".$empnumerooriginal."'" : 'Null');
				$set[] = "empanooriginal = ".($empanooriginal ? "'".$empanooriginal."'" : 'Null');
				
			}
			if( $xml->body->row->valor_ne ) 					$set[] = "empvalorempenho = '".$xml->body->row->valor_ne."'";
			if( $xml->body->row->ds_problema ) 					$set[] = "ds_problema = '".$xml->body->row->ds_problema."'";
			if( $xml->body->row->valor_total_empenhado )		$set[] = "valor_total_empenhado = '".$xml->body->row->valor_total_empenhado."'";
			if( $xml->body->row->valor_saldo_pagamento )		$set[] = "valor_saldo_pagamento = '".$xml->body->row->valor_saldo_pagamento."'";
			if( $xml->body->row->data_documento )				$set[] = "empdata = '".$xml->body->row->data_documento."'";
			if( $xml->body->row->unidade_gestora_responsavel )	$set[] = "empunidgestoraeminente = '".$xml->body->row->unidade_gestora_responsavel."'";
			//if( $xml->body->row->tp_especializacao )			$set[] = "tp_especializacao = '".$xml->body->row->tp_especializacao."'";
			//if( $xml->body->row->co_diretoria )					$set[] = "co_diretoria = '".$xml->body->row->co_diretoria."'";
			
			if($set){
				$sql = "UPDATE par.empenho SET ".(($set)?implode(",",$set):"")." 
							   WHERE empid='".$dados['empid']."'";
				$db->executar($sql);
			}
			
			$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, co_especie_empenho, ds_problema, valor_total_empenhado,valor_saldo_pagamento)
	    			VALUES ('".$_SESSION['usucpf']."',
	    					'".$dados['empid']."',
	    					NOW(),
	    					'".$situacaoEmpenho."',
	    					'".$xml->body->row->co_especie_empenho."',
	    					'".$xml->body->row->ds_problema."',
	    					".((strlen($xml->body->row->valor_total_empenhado))?"'".$xml->body->row->valor_total_empenhado."'":"NULL").",
	    					".((strlen($xml->body->row->valor_saldo_pagamento))?"'".$xml->body->row->valor_saldo_pagamento."'":"NULL").");";
			
			$db->executar($sql);
			$db->commit();
			
		} else {
			$hwpwebservice="consultarEmpenho - Erro";
			echo "------ CONSULTA DE EMPENHO ------\n\n";
			echo $xml->status->error->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->error->message->text)."\n\n";
		}
		
		$sql = "INSERT INTO par.historicowsprocessopar(
			    	prpid,
			    	hwpwebservice,
			    	hwpxmlenvio,
			    	hwpxmlretorno,
			    	hwpdataenvio,
			        usucpf)
			    VALUES ('".$prpid."',
			    		'".$hwpwebservice."',
			    		'".addslashes($arqXml)."',
			    		'".addslashes($xmlRetorno)."',
			    		NOW(),
			            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();
			
		if($result) {
			return false;
		} else {
		   	return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Consulta empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar empenho no SIGEF: $erroMSG";


	}
}

function solicitarConvenio($dados){
	global $db;		

	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	$xmlRetorno = "";

	$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prpnumeroconveniofnde, prpanoconveniofnde FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."' and prpstatus = 'A'");
	
	if($dadosse){
		$nu_cnpj_favorecido = pegaCnpj($_SESSION['par_var']['inuid']);
		/*
		$nu_cnpj_favorecido = $db->pegaUm("SELECT iue.iuecnpj 
	                                       FROM par.instrumentounidade iu
	                                               inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
	                                       WHERE
	                                               iu.inuid = {$dadosse['inuid']}
	                                           and iue.iuestatus = 'A'
	                                           and iue.iuedefault = true");*/
		//$nu_cnpj_favorecido = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$dadosse['inuid']);
	}

	$anoAtual     = date('Y');
	$dataAtualXml = date("c");
	
	if($nu_cnpj_favorecido){
			$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
<header>
<app>string</app>
<version>string</version>
<created>$dataAtualXml</created>
</header>
	<body>
	<params>
		<co_programa_fnde>03</co_programa_fnde>
		<an_exercicio>$anoAtual</an_exercicio>
	</params>
	</body>
</request>
XML;

		//if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		//	$urlWS = "http://172.20.200.116/webservices/sigefemendas/integracao/web/dev.php/convenio/consultar";						
		//} else {
			$urlWS = "http://www.fnde.gov.br/webservices/sigefemendas/index.php/convenio/consultar";
		//}
		try {

			$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef.convenio.consultar.request.xsd';
			$arMessage = validaXML( $arqXml, $xsd );
			
		    if( empty( $arMessage ) ){
		    	$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
					->execute();
		    } else {
		    	echo 'Erro-WS: validação xml - Consulta Convenio: <br>';
		    	foreach ($arMessage as $msg) {
		    		print_r( $msg['text'] );
		    	}
		    	die;
		    }
		//echo $xml;
		//echo '-------------';
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));

			if ( (int) $xml->status->result ){
		        $obConvenio = $xml->body->children();

		        $nu_convenio  = (int) $obConvenio->nu_convenio;
		        $an_exercicio = (int) $obConvenio->an_exercicio;

				if($nu_convenio && $an_exercicio){
					# Atualiza a tabela de processopar.
					$sql = "update par.processopar set prpnumeroconveniofnde = {$nu_convenio}, prpanoconveniofnde =  {$an_exercicio} where prpid = '".$_SESSION['par_var']['prpid']."'";
					$db->executar($sql);
			
				} else {
					throw new Exception('Não foi possível recuperar Número e o Ano do convênio.');
				}
		    } else {
		    	throw new Exception(utf8_decode($xml->status->error->message->text));
		    }

		} catch (Exception $e){
			/**
			 * @var LogErroWS
			 * Bloco que grava o erro em nossa base
			 */

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarConvenio - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
	
			$db->executar($sql);
			$db->commit();
			echo "\n\n------ CONSULTAR NÚMERO DE CONVÊNIO ------\n\n";
			
			# Erro 404 página not found
			if($e->getCode() == 404){
				echo "Erro-Serviço Convênio encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				return false;
				die;
			}
			
			echo 'Erro-WS Consulta Convenio: '.$e->getMessage();
			return false;
			die;
		}	
	}
	$db->commit();
	return $obConvenio;
}

?>
