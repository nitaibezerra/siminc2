<?php

verificaSessao( 'idobra' );

$flcid = ($_GET['flcid'] ? $_GET['flcid'] : '');

$faseLicitacao = new FaseLicitacao( $flcid );
extract( $faseLicitacao->getDados() );

$obraLicitacao = new ObraLicitacao();
$licid = ($_SESSION["obras2"]['licid']) ? $_SESSION["obras2"]['licid'] : $obraLicitacao->pegaLicidPorObrid($_SESSION['obras2']['obrid']);

switch($_REQUEST['requisicao']) {
	case 'salvar':
		$dados 			= $_POST;
            
            if(strlen($dados['flcobshomol']) > 500){
                $dados['flcobshomol'] = substr($dados['flcobshomol'], 0, 499);
            }
		
		$arquivo_1 = $_FILES["arquivo_1"];
//	    $arqid_1   = "";
	    if ( $_FILES["arquivo_1"] && $arquivo_1["name"] && $arquivo_1["type"] && $arquivo_1["size"] ){
			include_once APPRAIZ."includes/classes/fileSimec.class.inc";
			
			$file = new FilesSimec('', '', 'obras2');
	        $file->setPasta('obras2');
	        $file->setUpload(null, 'arquivo_1', false);
	        $dados['arqid'] = $file->getIdArquivo();
	    }else{
	    	$dados['arqid'] = null;
	    }
	    
		$faseLic = new FaseLicitacao( $_POST['flcid'] );
		$flcid 	 = $faseLic->popularDadosObjeto( $dados )
                                   ->salvar();
		if($_GET['rca']){
                    $faseLicitacao = new FaseLicitacao( $_POST['flcid'] );
                    $sql = "SELECT
                            distinct obrid
                    FROM
                            obras2.obralicitacao ol
                    JOIN obras2.faselicitacao fl ON fl.licid = ol.licid AND
                    fl.flcstatus = 'A'
                    WHERE fl.flcid = {$flcid} AND ol.oblstatus = 'A'";

                    $obras = $db->carregar($sql);

                    foreach ($obras as $obra) {
                         $obra = new Obras($obra['obrid']);
                         registraAtividade(
                                        'Inclusão de Fase da Licitação Obra: ('.$obra->obrid.') ' . $obra->obrnome, 
                                        $_POST['rgadsccompleta'], 
                                        $arqidcusto);
                    } 
                }
	    $arquivo = $_FILES["arquivo"];
	    $arqid   = "";
	    if ( $_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"] ){
			include_once APPRAIZ."includes/classes/fileSimec.class.inc";
			
			$campos	= array(
					"flcid" 	    => $flcid,
					"usucpf"    	=> "'".$_SESSION['usucpf']."'"
					);	
			$file = new FilesSimec("arquivolicitacao", $campos ,"obras2");
	        $file->setPasta('obras2');
	        $file->setUpload($_POST['arqdescricao'], 'arquivo', true);
	        $arqid = $file->getIdArquivo();
	    }

	    if ( empty($licid) ){
		    $_SESSION['obras2']['idFaseLicitacao']   = (is_array($_SESSION['obras2']['idFaseLicitacao']) ? $_SESSION['obras2']['idFaseLicitacao'] : array());
		    $_SESSION['obras2']['idFaseLicitacao'][] = $flcid; 
	    }	
	    			 
		$db->commit();
		
                if($_GET['rca']){
                    echo "<script>
                                    alert('Operação realizada com sucesso!');
                                    window.opener.location = '/obras2/obras2.php?modulo=principal/popupEditarLicitacao&acao=E&obrid={$_SESSION['obras2']['obrid']}';
                                    window.close();
                              </script>";exit;
                } else {
                
                    echo "<script>
                                    alert('Operação realizada com sucesso!');
                                    " . 
                                    ($licid 
                                            ? "window.opener.location = '?modulo=principal/cadLicitacao&acao=E';"
                                            : "window.opener.location = '?modulo=principal/cadLicitacao&acao=A&fase=true';")
                                    . "
                                    window.close();
                              </script>";
                    exit();
                }
	break;
	case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die();
	case "excluir":
		$arqid = $_GET['arqid'];
		if ( $arqid ){
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $arquivoLicitacao = new ArquivoLicitacao();
            $dados = $arquivoLicitacao->buscaDadosPorArqid($arqid);
            $arquivoLicitacao->inativar($dados['aqlid']);

//			$file 		  = new FilesSimec("arquivolicitacao", array(''), "obras2");
//			$file->setRemoveUpload($arqid);
			$msg = 'Operação realizada com sucesso!';

		}else{
			$msg = 'Falha na execução da operação!';
		}
		die('<script>
				alert(\'' . $msg . '\');
				window.close();
			 </script>');
	break;
}

$dadoArquivo = $faseLicitacao->getArquivo( $flcid );

//switch($_REQUEST['requisicao']) {
//	case 'salvar':
//		if( empty($_GET['flcid']) ) $licitacao->salvarFasesLicitacao( $_POST );
//		if( $_FILES['arquivo']['name'] ){
//			$dir = 'fases_licitacao&acao=A';
//			if( $_POST['tflid'] == 9 ) $_POST['tpaid'] = 24;
//			if( $_POST['tflid'] == 2 ) $_POST['tpaid'] = 3;
//			$obras->EnviarArquivo($_FILES, $_POST, $dir, false);
//		}
//		if($db->commit()){
//			echo "<script>
//				alert('Operação realizada com sucesso!');
//				window.opener.location.reload();
//				window.close();</script>";
//		}
//		exit();
//	break;
//	case "download":
//		$obras->DownloadArquivo( $_REQUEST );
//	break;
//	case "excluir":
//		$dir = 'principal/fases_licitacao';
//		$obras->DeletarDocumento( $_REQUEST, $dir, false );
//		echo "<script>
//				alert('Operação realizada com sucesso!');
//				window.close();</script>";
//	break;
//}

$habil = ( $_REQUEST['acao'] == 'V' ? 'N' : 'S' );

$titulo_modulo = "Fases de Licitação";
monta_titulo( $titulo_modulo, 'Selecione a fase desejada' );

?>
<html>
	<head>
		<title>Fases da Licitação</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		<script src="/includes/calendario.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
<!--
function populaValoresCamposFaseLicitacao(id){
	
	var form = document.formulario;
	for(i = 0 ; i < form.elements.length ; i++){
		var CampoAtual = form.elements[i];
		if(CampoAtual.name =="flcpubleditaldtprev"){
			CampoAtual.value = window.opener.document.getElementById("flcpubleditaldtprev_"+id+"").value;
		}
		if(CampoAtual.name =="flcdtrecintermotivo"){
			CampoAtual.value = window.opener.document.getElementById("flcdtrecintermotivo_"+id+"").value;
		}

		if(CampoAtual.name =="flcrecintermotivo"){
			CampoAtual.value = window.opener.document.getElementById("flcrecintermotivo_"+id+"").value;
		}

		if(CampoAtual.name =="flcordservdt"){
			CampoAtual.value = window.opener.document.getElementById("flcordservdt_"+id+"").value;
		}
		
		if(CampoAtual.name =="flcordservnum"){
			CampoAtual.value = window.opener.document.getElementById("flcordservnum_"+id+"").value;
		}
		
		if(CampoAtual.name =="flchomlicdtprev"){
			CampoAtual.value = window.opener.document.getElementById("flchomlicdtprev_"+id+"").value;
		}
		
		if(CampoAtual.name =="flcaberpropdtprev"){
			CampoAtual.value = window.opener.document.getElementById("flcaberpropdtprev_"+id+"").value;
		}
		
		if(CampoAtual.name =="flcobshomol"){
			CampoAtual.value = window.opener.document.getElementById("flcobshomol_"+id+"").value;
		}
		
		if(CampoAtual.name =="flcmeiopublichomol"){
			CampoAtual.value = window.opener.document.getElementById("flcmeiopublichomol_"+id+"").value;
		}						
	}					
}

function abreCamposFaseLicitacao(id){
	var tr_public 		 = document.getElementById( 'publicacao' );
	var div_anexos 		 = document.getElementById( 'anexos' );
	var tr_rec  		 = document.getElementById( 'recurso' );
	var tr_ord 			 = document.getElementById( 'ordem_de_servico' );
	var tr_hom 			 = document.getElementById( 'homologacao' );
	var tr_pub  		 = document.getElementById( 'meio_publicacao' );
	var tr_obs  		 = document.getElementById( 'observacao' );
	var tr_aberprop 	 = document.getElementById( 'abertura_de_proposta' );
	var tr_anexo_decreto = document.getElementById( 'anexo_decreto' );

	tr_anexo_decreto = tr_anexo_decreto || null;
	
	if ( tr_anexo_decreto != null )
		tr_anexo_decreto.style.display = 'none';
	
	if(id == ''){
		if (document.selection){
			tr_public.style.display = 'none';
			div_anexos.style.display = 'none';
			tr_rec.style.display = 'none';
			tr_ord.style.display = 'none';
			tr_hom.style.display = 'none';
			tr_pub.style.display = 'none';
			tr_obs.style.display = 'none';
			tr_aberprop.style.display = 'none';
		}else{
			tr_public.style.display = 'none';
			div_anexos.style.display = 'none';
			tr_rec.style.display = 'none';
			tr_ord.style.display = 'none';
			tr_hom.style.display = 'none';
			tr_pub.style.display = 'none';
			tr_obs.style.display = 'none';
			tr_aberprop.style.display = 'none';
		}
	}
	
	if(id == 2){
		if (document.selection){
			tr_public.style.display = 'block';
			div_anexos.style.display = 'block';
			tr_rec.style.display = 'none';
			tr_ord.style.display = 'none';
			tr_hom.style.display = 'none';
			tr_pub.style.display = 'none';
			tr_obs.style.display = 'none';
			tr_aberprop.style.display = 'none';
			document.formulario.flcdtrecintermotivo.value = '';
			document.formulario.flcrecintermotivo.value = '';
			document.formulario.flcordservdt.value = '';
			document.formulario.flcordservnum.value = '';
			document.formulario.flchomlicdtprev.value = '';
			document.formulario.flcaberpropdtprev.value = '';
		}else{
			tr_public.style.display = 'table-row';
			div_anexos.style.display = '';
			tr_rec.style.display = 'none';
			tr_ord.style.display = 'none';
			tr_hom.style.display = 'none';
			tr_pub.style.display = 'none';
			tr_obs.style.display = 'none';
			tr_aberprop.style.display = 'none';
		}
	}
	if(id == 5){
		if (document.selection){
			tr_rec.style.display = 'block';
			tr_public.style.display = 'none';
			div_anexos.style.display = 'none';
			tr_ord.style.display = 'none';
			tr_hom.style.display = 'none';
			tr_pub.style.display = 'none';
			tr_obs.style.display = 'none';
			tr_aberprop.style.display = 'none';
			document.formulario.flcpubleditaldtprev.value = '';
			document.formulario.flcordservdt.value = '';
			document.formulario.flcordservnum.value = '';
			document.formulario.flchomlicdtprev.value = '';
			document.formulario.flcaberpropdtprev.value = '';
		}else{
			tr_rec.style.display = 'table-row';
			tr_public.style.display = 'none';
			div_anexos.style.display = 'none';
			tr_ord.style.display = 'none';
			tr_hom.style.display = 'none';
			tr_pub.style.display = 'none';
			tr_obs.style.display = 'none';
			tr_aberprop.style.display = 'none';
		}
	}
	if(id == 6){
		if (document.selection){
			tr_rec.style.display = 'none';
			tr_public.style.display = 'none';
			div_anexos.style.display = 'none';
			tr_ord.style.display = 'block';
			tr_hom.style.display = 'none';
			tr_pub.style.display = 'none';
			tr_obs.style.display = 'none';
			tr_aberprop.style.display = 'none';
			document.formulario.flcpubleditaldtprev.value = '';
			document.formulario.flcdtrecintermotivo.value = '';
			document.formulario.flcrecintermotivo.value = '';
			document.formulario.flchomlicdtprev.value = '';
			document.formulario.flcaberpropdtprev.value = '';
		}else{
			tr_rec.style.display = 'none';
			tr_public.style.display = 'none';
			div_anexos.style.display = 'none';
			tr_ord.style.display = 'table-row';
			tr_hom.style.display = 'none';
			tr_pub.style.display = 'none';
			tr_obs.style.display = 'none';
			tr_aberprop.style.display = 'none';
		}
	}
	if(id == 9){
		if (document.selection){
			tr_rec.style.display = 'none';
			tr_public.style.display = 'none';
			tr_ord.style.display = 'none';
			tr_hom.style.display = 'block';
			tr_pub.style.display = 'block';
			tr_obs.style.display = 'block';
			div_anexos.style.display = 'block';
			tr_aberprop.style.display = 'none';
			
			if ( tr_anexo_decreto != null )
				tr_anexo_decreto.style.display = 'block';
			
			document.formulario.flcpubleditaldtprev.value = '';
			document.formulario.flcdtrecintermotivo.value = '';
			document.formulario.flcrecintermotivo.value = '';
			document.formulario.flcordservdt.value = '';
			document.formulario.flcordservnum.value = '';
			document.formulario.flcaberpropdtprev.value = '';
		}else{
			tr_rec.style.display = 'none';
			tr_public.style.display = 'none';
			tr_ord.style.display = 'none';
			tr_hom.style.display = 'table-row';
			tr_pub.style.display = 'table-row';
			tr_obs.style.display = 'table-row';
			div_anexos.style.display = '';
			tr_aberprop.style.display = 'none';
			
			if ( tr_anexo_decreto != null )
				tr_anexo_decreto.style.display = 'table-row';
		}
	}
	if(id == 7){
		if (document.selection){
			tr_rec.style.display = 'none';
			tr_public.style.display = 'none';
			div_anexos.style.display = 'none';
			tr_ord.style.display = 'none';
			tr_hom.style.display = 'none';
			tr_pub.style.display = 'none';
			tr_obs.style.display = 'none';
			tr_aberprop.style.display = 'block';
			document.formulario.flcpubleditaldtprev.value = '';
			document.formulario.flcdtrecintermotivo.value = '';
			document.formulario.flcrecintermotivo.value = '';
			document.formulario.flcordservdt.value = '';
			document.formulario.flcordservnum.value = '';
			document.formulario.flchomlicdtprev.value = '';
		}else{
			tr_rec.style.display = 'none';
			tr_public.style.display = 'none';
			div_anexos.style.display = 'none';
			tr_ord.style.display = 'none';
			tr_hom.style.display = 'none';
			tr_pub.style.display = 'none';
			tr_obs.style.display = 'none';
			tr_aberprop.style.display = 'table-row';
		}
	}
}
-->
</script>
<script type="text/javascript"><!--
			
			function salvafases(){
				var form = window.document.getElementById('formulario');
				//var formOpener = window.opener.document.getElementById("formulario");
				//var flcid = window.opener.document.getElementById("flcid");
				
				var flag = "";
				var add = false;
				var osNum = null;
				var flcrecintermotivoDesc = null;
				var flcmeiopublichomolDesc = null;
				var flcobshomolDesc = null;
				var boVazio = false;
								
				for(i = 0 ; i < form.elements.length ; i++){
					
					var CampoAtual = form.elements[i];
										
					if(CampoAtual.type == "select-one" ){
						var index = CampoAtual.selectedIndex;
						combo_id = CampoAtual.options[index].value;
						tr_id = CampoAtual.options[index].value;
						combo_text = CampoAtual.options[index].text;
					}
					if(CampoAtual.name.indexOf("dt") != -1){
						if(CampoAtual.value.length > 0){
							data_value = CampoAtual.value;
							data_name = CampoAtual.name;
						}
					}
					if(CampoAtual.name.indexOf("flcordservnum") != -1){
						if(CampoAtual.value.length > 0){
							osNum = CampoAtual.value;
							osName = CampoAtual.name;
						}
					}
					if(CampoAtual.name == "flcrecintermotivo" ){
						if(CampoAtual.value.length > 0){
							flcrecintermotivoDesc = CampoAtual.value;
						}
					}
					if(CampoAtual.name == "flcmeiopublichomol" ){
						if(CampoAtual.value.length > 0){
							flcmeiopublichomolDesc = CampoAtual.value;
						} else {
							boVazio = true;
						}
					}
					if(CampoAtual.name == "flcobshomol" ){
						if(CampoAtual.value.length > 0){
							flcobshomolDesc = CampoAtual.value;
						}
					}				
					
				}
				add = true;
				// Verifica se a data foi preenchda
				try{
					document.formulario.data_form.value=data_value;
					//alert(data_value+" - "+data_name);
				}
				catch(e){
					add = false;
					window.alert("Favor preencher a Data.");
					return false;
				}
				
				if( boVazio && window.document.getElementById("tflid").value == 9 ){
					alert('Favor preencher o Meio de Publicação.');
					return false;
				}
				
				/*if( window.document.getElementById("flcid").value == 2 || window.document.getElementById("flcid").value == 9 ){
					if( !validaAnexos() ) return false;
				}*/
				
				novo=window.document.getElementById("flcid_f");
				if (novo.value != "")
					novo=false;
				else
					novo=true;
				if(novo){
					// Verifica se a data do registro está maior que a última data inserida				
					try{
						na_tela_flcdata=window.opener.document.getElementsByName("flcdata[]");
						data_opener=na_tela_flcdata[parseInt(na_tela_flcdata.length)-1].value;
						document.formulario.data_opener.value=data_opener;
						data_form=document.getElementById("data_form");
						data_opener=document.getElementById("data_opener");
						//alert(data_form.value +"-"+data_opener.value);
						if (!validaDataMaior(data_opener, data_form)){
							
							add = false;
							msg_erro="A data deve ser maior que a última data inserida.";
						}
					}catch(e){
						//window.alert("Data");
				  	}
					// Verificando se o registro é o único
					try{
						// Verifica se o registro já consta nas fases de contratação
						na_tela_tflid=window.opener.document.getElementsByName("tflid[]");
						for(i = 0 ; i < na_tela_tflid.length ; i++){
							if(na_tela_tflid[i].value == combo_id){
								add = false;
								msg_erro="Item já inserido.";
								i=na_tela_tflid.length;
							}
						} 
					}catch(e){
						//window.alert("Unico");
				  	}
			  	}
				
				//Verifica se a data é maior que e tem pelo menos um anexo quando for homologação.
				if(jQuery("#tflid").val() == 9)
				{
					var data1 = "31/08/2011";
					var data2 = jQuery("#flchomlicdtprev").val();
					if ( parseInt( data2.split( "/" )[2].toString() + data2.split( "/" )[1].toString() + data2.split( "/" )[0].toString() ) > parseInt( data1.split( "/" )[2].toString() + data1.split( "/" )[1].toString() + data1.split( "/" )[0].toString() ) )
					{
						 if( !jQuery("[name='formlista']").html() &&  !jQuery("[name='arquivo']").val())
						 {
							alert('Favor inserir pelo menos um arquivo.');
							return false;
						 }
					}
					
//					 if( !jQuery("[name='arquivo_1']").val() && !jQuery("#flcid").val()){
					 if( !jQuery("[name='arquivo']").val() && !jQuery("#flcid").val()){
						alert('Favor inserir pelo menos um arquivo.');
						return false;
					 }
				}
				
				window.document.getElementById('requisicao').value = 'salvar';
				window.document.getElementById("formulario").submit();
				/*window.close();
				return true;*/
			}
			
			function BuscaCampo(form,camponame){
				for(i = 0 ; i < form.elements.length ; i++){
					var CampoAtual = form.elements[i];
					if(CampoAtual.name == camponame){
						return true;
					}else{
						return false;
					}
				}
			}
			
			function validaAnexos(){
				var objForm = document.forms["formulario"];
				var alerta = "";
				if(objForm.arquivo.value == '' && objForm.arqid.value == ''){
					alerta += "Você deve escolher um arquivo.\n";
				}
				if(objForm.arqdescricao.value == ''){
					alerta += "O campo Descrição Anexo é Obrigatório.";
				}
				
				if(alerta){
					alert(alerta);
					return false;
				} 
				return true;
			}
			
			function ExcluirDocumento(url, arqid, aqlid){
				if(confirm("Deseja realmente excluir este arquivo?")){
					window.location = url+'&aqoid='+aqlid+'&arqid='+arqid;
				}
			}
			DownloadArquivo = function(arqid){
				window.location = '?modulo=principal/vincFaseLicitacao&acao=A&requisicao=download&arqid='+arqid;
			}
			
			
		--></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
			<input type="hidden" name="flcid" id="flcid" value="<?php echo $flcid ?>"/>
			<input type="hidden" name="licid" id="licid" value="<?php echo $licid ?>">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="subtitulodireita">Selecione a Fase</td>
					<td>
						<?php
						$tipoFaseLic = new TiposFasesLicitacao();
						$db->monta_combo('tflid', $tipoFaseLic->listaCombo($licid, $tflid, $_SESSION['obras2']['idFaseLicitacao']), $habil, "Selecione...", 'abreCamposFaseLicitacao', '', '', null, 'S', 'tflid'); 
						?>
					</td>
				</tr>
				<tr id="publicacao" style="display: none;">
					<td class="SubTituloDireita">Data</td>
					<td>
					<?
					echo campo_data2( 'flcpubleditaldtprev', 'S', $habil, '', 'S' ); 
					?>
					</td>
				</tr>
				<tr id="recurso" style="display: none;">
					<td align="center" colspan="2">
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td class="SubTituloDireita">Data</td>
								<td>
								<? 
								echo campo_data2( 'flcdtrecintermotivo', 'S', $habil, '', 'S' ); 
								?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Motivo</td>
								<td>
								<?= campo_textarea( 'flcrecintermotivo', 'N', $habil, '', '50', '4', '500'); ?>
								</td>
							</tr>
						</table>						
					</td>
				</tr>
				<tr id="ordem_de_servico" style="display: none;">
					<td align="center" colspan="2">
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td class="SubTituloDireita">Data da O.S.</td>
								<td>
								<?
								echo campo_data2( 'flcordservdt', 'S', $habil, '', 'S' );
								?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Número da O.S.</td>
								<td>
								<?= campo_texto( 'flcordservnum', 'N', $habil, '', 17, 15, '',  '', 'left', '', 0); ?>
								</td>
							</tr>
						</table>
					</td>
				</tr>
				<tr id="homologacao" style="display: none;">
					<td class="SubTituloDireita">Data</td>
					<td>
					<?php
					echo campo_data2( 'flchomlicdtprev', 'S', $habil, '', 'S' ); 
					?>
					</td>
				</tr>
				<tr id="meio_publicacao" style="display: none;">
					<td class="SubTituloDireita">Meio de Publicação:</td>
					<td>
					<?php
					echo campo_texto( 'flcmeiopublichomol', 'S', $habil, '', 60, 100, '',  '', 'left', '', 0);
					?>
					</td>
				</tr>
				<tr id="observacao" style="display: none;">
					<td class="SubTituloDireita">Observação:</td>
					<td>
					<?php
					echo campo_textarea( 'flcobshomol', 'N', $habil, '', '84', '4', '500');
					?>
					</td>
				</tr>
				<tr id="abertura_de_proposta" style="display: none;">
					<td class="SubTituloDireita">Data</td>
					<td>
					<?php
					echo campo_data2( 'flcaberpropdtprev', 'S', $habil, '', 'S' );
					?>
					</td>
				</tr>
			</table>
			<input type=hidden name="data_form" id="data_form" value="">
			<input type=hidden name="data_opener" id="data_opener" value="">
			<input type='hidden' name='flcid_f' id='flcid_f' value='<?=$flcid?>'/>
		<div id="anexos" style="display: none;">
		<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />
		<input type="hidden" name="requisicao" id="requisicao" value="" />
		<input type="hidden" name="tpaid" id="tpaid" value="" />
		<?php  
		if ( $habil == 'S' ):
		?>
		<table class="tabela" bgcolor="#f5f5f5" width="95%" cellspacing="1" cellpadding="3" align="center">
			<tr>
				<th colspan="2">Anexar Documentos</th>
			</tr>
			<?php
			if ( $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_BASICA ):
			?>
			<tr id="anexo_decreto" style="display: none;">
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">
					Declaração de acordo com o decreto nº XXXX. (o XXX ainda vai ser especificado):
				</td>
				<td>
					<?php
					if ( $dadoArquivo['arqid'] ){
					?>
					<a style="color: blue; padding-top:5px;" href="javascript:DownloadArquivo(<?=$dadoArquivo['arqid']?>);"><?=$dadoArquivo['arqnome'] . '.' . $dadoArquivo['arqextensao'] ?></a>
					<br>
					<br>
					<?php
					}
					?>
					<input type="file" name="arquivo_1" id="arquivo_1"/>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita" style="" colspan="2">&nbsp;</td>
			</tr>
			<?php
			endif;
			?>
			<tr>
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
				<td>
					<input type="file" name="arquivo" id="arquivo"/>
					<?=obrigatorio(); ?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
				<td><?= campo_textarea( 'arqdescricao', 'N', 'S', '', 50, 2, 250 ); ?></td>
			</tr>
		</table>
		<?php 
		endif;
		?>
		</form>
		<?
		if ($flcid){
		$arquivoLic = new ArquivoLicitacao();		
		
		$cabecalho = array( "Ação", 
							"Data Inclusão",
//							"Tipo Arquivo",
							"Nome Arquivo",
							"Tamanho (Mb)",
							"Descrição Arquivo",
							"Responsável");
		$db->monta_lista( $arquivoLic->listaSqlPorFaseLicitacao( $flcid, $habil ), $cabecalho, 50, 10, 'N', '', '' );
		}
	?>
	</div>
	<table class="tabela" bgcolor="#f5f5f5" width="95%" cellspacing="1" cellpadding="3" align="center">
		<tr bgcolor="#C0C0C0">
			<td width="33%">
				&nbsp;
			</td>
			<td>
				<div style="float: left;">
					<?php 
					if ( $habil == 'S' ):
					?>
					<input type="button" onclick="salvafases();" name="btn_salvar" value="Salvar">
					<?php 
					else:
					?>
					<input type="button" onclick="window.close();" name="btn_fechar" value="Fechar">
					<?php 
					endif;
					?>
				</div>
			</td>
		</tr>
	</table>
	</body>
</html>
<div id="erro"></div>

<?php
if($tflid){
	echo "<script>";
	echo "	abreCamposFaseLicitacao(".$tflid."); ";
	echo "	populaValoresCamposFaseLicitacao('".$flcid."');";
	echo "</script>";
}
?>
<?

function registraAtividade($descricaocurta, $descricao, $arqid = null, $rgaid = null){
    global $db;
    
    $regAtividade = new RegistroAtividade();
    $arDado = array();

    $arDado['arqid'] = $arqid;
    $arDado['obrid'] = $_SESSION['obras2']['obrid'];
    $arDado['usucpf'] = $_SESSION['usucpf'];
    $arDado['rgadscsimplificada'] = $descricaocurta;
    $arDado['rgadsccompleta'] = $descricao;

    $arCamposNulo = array();

    if ( empty($arDado['arqid']) )
        $arCamposNulo[] = 'arqid'; 	

    $rga = $regAtividade->popularDadosObjeto( $arDado )
                 ->salvar(true, true, $arCamposNulo);
    $db->commit();
    
    return $rga;
}
if(possui_perfil(PFLCOD_CALL_CENTER) ){
    $blockEdicao = TRUE;
}
if($blockEdicao){
    echo '<script type="text/javascript">';
    echo " setTimeout(bloqueiaForm('formulario'), 500);
           function bloqueiaForm(idForm){
              jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
              jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
              jQuery('#voltar').attr('disabled', false);
           }
         ";
    echo '</script>';
}
?>

