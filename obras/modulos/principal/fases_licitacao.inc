<?php

$obras = new Obras();
$licitacao = new licitacao();

switch($_REQUEST['requisicao']) {
	case 'salvar':
		if( empty($_GET['flcid']) ) $licitacao->salvarFasesLicitacao( $_POST );
		if( $_FILES['arquivo']['name'] ){
			$dir = 'fases_licitacao&acao=A';
			if( $_POST['tflid'] == 9 ) $_POST['tpaid'] = 24;
			if( $_POST['tflid'] == 2 ) $_POST['tpaid'] = 3;
			$obras->EnviarArquivo($_FILES, $_POST, $dir, false);
		}
		if($db->commit()){
			echo "<script>
				alert('Operação realizada com sucesso!');
				window.opener.location.reload();
				window.close();</script>";
		}
		exit();
	break;
	case "download":
		$obras->DownloadArquivo( $_REQUEST );
	break;
	case "excluir":
		$dir = 'principal/fases_licitacao';
		$obras->DeletarDocumento( $_REQUEST, $dir, false );
		echo "<script>
				alert('Operação realizada com sucesso!');
				window.close();</script>";
	break;
}

$titulo_modulo = "Fases de Licitação";
monta_titulo( $titulo_modulo, 'Selecione a fase desejada' );

if($_SESSION["obra"]["obrid"]){	
	$dados = $obras->Dados($_SESSION["obra"]["obrid"]);
	$dobras = new DadosObra($dados);
}     
else{
	 echo "<script>
				alert('Sessão da Obra expirou. Entre novamente!');
				location.href='obras.php?modulo=inicio&acao=A';
		   </script>";
}
?>
<html>
	<head>
		<title>Fases da Licitação</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script src="../includes/calendario.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
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

				function abreCamposFaseLicitacao(id)
				{
				var tr_public  = document.getElementById( 'publicacao' );
				var div_anexos  = document.getElementById( 'anexos' );
				var tr_rec  = document.getElementById( 'recurso' );
				var tr_ord  = document.getElementById( 'ordem_de_servico' );
				var tr_hom  = document.getElementById( 'homologacao' );
				var tr_pub  = document.getElementById( 'meio_publicacao' );
				var tr_obs  = document.getElementById( 'observacao' );
				var tr_aberprop = document.getElementById( 'abertura_de_proposta' );
				
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
				
				if( boVazio && window.document.getElementById("flcid").value == 9 ){
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
				/*if(add){
					
					var tabela = window.opener.document.getElementById("faseslicitacao");
					var flcid = document.getElementById("flcid_f");
					
					if(flcid.value == ""){
						var flcidNum = "_A"+Math.random(10);
						var tamanho = tabela.rows.length;
						var tr = tabela.insertRow(tamanho);	
						tr.id = "tr_"+flcidNum;
					}else{
						var flcidNum =flcid.value;
						linhaid = window.opener.document.getElementById('tr_'+flcidNum).rowIndex;
						tabela.deleteRow(linhaid);
						tr = tabela.insertRow(linhaid);
						tr.id = "tr_"+flcidNum;
					}
										
					var colAcao = tr.insertCell(0);
					var colDesc = tr.insertCell(1);
					var colData = tr.insertCell(2);
					
					colAcao.style.textAlign = "center";
				
					var botao =   ""
						+ "<img src='/imagens/alterar.gif' style='cursor: pointer' border=0 title='Editar' onclick=\"atualizaFase(" + combo_id + ",'"+flcidNum+"')\";/>&nbsp&nbsp&nbsp"
						+ "<img src='/imagens/excluir.gif' style='cursor: pointer'  border=0 title='Excluir' onclick='RemoveLinha(window.document.getElementById(\"tr_"+flcidNum+"\").rowIndex,\""+flcidNum+"\");'>"; 
													
					colAcao.innerHTML = botao;						
						
					colDesc.innerHTML = '<input type="hidden" name="tflid[]" id="tflid_'+flcidNum+'" value="' + combo_id + '">' + combo_text;
					colData.innerHTML = '';
					
					if(data_name == "flcpubleditaldtprev"){
						colData.innerHTML += '<input type="hidden" name="flcpubleditaldtprev[]" id="flcpubleditaldtprev_'+flcidNum+'" value="' + data_value + '">';
						if( window.opener.document.getElementById('dtiniciolicitacao') ){
							window.opener.document.getElementById('dtiniciolicitacao').value = data_value;
						}
					}else{
						colData.innerHTML += '<input type="hidden" name="flcpubleditaldtprev[]" id="flcpubleditaldtprev_'+flcidNum+'" value="">';
					}
					if(data_name =="flcdtrecintermotivo"){
						colData.innerHTML += '<input type="hidden" name="flcdtrecintermotivo[]" id="flcdtrecintermotivo_'+flcidNum+'" value="' + data_value + '">';
					}else{
						colData.innerHTML += '<input type="hidden" name="flcdtrecintermotivo[]" id="flcdtrecintermotivo_'+flcidNum+'" value="">';						
					}
					if(data_name =="flcordservdt"){
					colData.innerHTML += '<input type="hidden" name="flcordservdt[]" id="flcordservdt_'+flcidNum+'" value="' + data_value + '">';
					}else{
					colData.innerHTML += '<input type="hidden" name="flcordservdt[]" id="flcordservdt_'+flcidNum+'" value="">';						
					}
					if(data_name =="flchomlicdtprev"){
					colData.innerHTML += '<input type="hidden" name="flchomlicdtprev[]" id="flchomlicdtprev_'+flcidNum+'" value="' + data_value + '">';
					}else{
					colData.innerHTML += '<input type="hidden" name="flchomlicdtprev[]" id="flchomlicdtprev_'+flcidNum+'" value="">';						
					}
					if(data_name =="flcaberpropdtprev"){
					colData.innerHTML += '<input type="hidden" name="flcaberpropdtprev[]" id="flcaberpropdtprev_'+flcidNum+'" value="' + data_value + '">';
					}else{
					colData.innerHTML += '<input type="hidden" name="flcaberpropdtprev[]" id="flcaberpropdtprev_'+flcidNum+'" value="">';						
					}
					
					colData.innerHTML += '' + data_value;
					colData.innerHTML += '<input type="hidden" name="flcid[]" id="flcid_'+flcidNum+'" value="'+flcidNum+'">';
					colData.innerHTML += '<input type="hidden" name="flcordservnum[]" id="flcordservnum_'+flcidNum+'" value="' + osNum + '">';
					colData.innerHTML += '<input type="hidden" name="flcdata[]" id="flcdata'+flcidNum+'" value="'+data_value+'">';
					colData.innerHTML += '<input type="hidden" name="flcrecintermotivo[]" id="flcrecintermotivo_'+flcidNum+'" value="'+flcrecintermotivoDesc+'">';
					colData.innerHTML += '<input type="hidden" name="flcmeiopublichomol[]" id="flcmeiopublichomol_'+flcidNum+'" value="'+flcmeiopublichomolDesc+'">';
					colData.innerHTML += '<input type="hidden" name="flcobshomol[]" id="flcobshomol_'+flcidNum+'" value="'+flcobshomolDesc+'">';
					//if( window.document.getElementById("flcid").value != 2 && window.document.getElementById("flcid").value != 9 ){
						window.close();	
					//}
				}else{
					alert(msg_erro);
					return false;
				}*/
				
				//Verifica se a data é maior que e tem pelo menos um anexo quando for homologação.
				if(jQuery("#flcid").val() == 9)
				{
					var data1 = "31/08/2011";
					var data2 = jQuery("#flchomlicdtprev").val();
					if ( parseInt( data2.split( "/" )[2].toString() + data2.split( "/" )[1].toString() + data2.split( "/" )[0].toString() ) > parseInt( data1.split( "/" )[2].toString() + data1.split( "/" )[1].toString() + data1.split( "/" )[0].toString() ) )
					{
						 if( !jQuery("[name='formlista']").html() &&  !jQuery("[name='arquivo']").val())
						 {
							alert('Favor inserir pelo menos um arquivo!');
							return false;
						 }
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
			
		--></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="subtitulodireita">Selecione a Fase</td>
					<td>
						<?php
							$novo=true;
							if ($_REQUEST["tflid"]){
								$tflid = $_REQUEST["tflid"];
								$novo=false;
							}
							// Filtra os registros de acordo com a tela chamada
							$stFiltro = '';
							if ( $_REQUEST['objeto'] ) {
								switch ( $_REQUEST['objeto'] ){
									case 'licitacao' : $stFiltro = " and tflexibefl = true AND tflstatus = 'A' "; break;
									case 'projeto'   : $stFiltro = " and tflexibefp = true AND tflstatus = 'A' "; break;
								}
							}
							if( !empty($_GET['flcid']) ){
								$sql = "SELECT tflid AS codigo, tfldesc AS descricao 
									FROM obras.tiposfaseslicitacao ".$stFiltro." ORDER BY tflordem ";
							} else {
								if($_SESSION['obra']['obrid']){
									$sql = "SELECT tf.tflid AS codigo, tf.tfldesc AS descricao 
											FROM obras.tiposfaseslicitacao tf
		                                    WHERE tf.tflid not in (select tflid from obras.faselicitacao a where a.obrid = {$_SESSION['obra']['obrid']} and a.flcstatus = 'A' and a.tflid = tf.tflid)
		                                    	".$stFiltro." ORDER BY tf.tflordem";
								}else{
									$sql = array();
								}
							}
							
							//if ($novo)
								$db->monta_combo('tflid', $sql, 'S', "Selecione...", 'abreCamposFaseLicitacao', '', '', null, 'S', 'flcid'); 
							//else
								//$db->monta_combo('tflid', $sql, 'N', "Selecione...", 'abreCamposFaseLicitacao', '', '', null, 'S', 'flcid');							
						?>
					</td>
				</tr>
				<tr id="publicacao" style="display: none;">
					<td class="SubTituloDireita">Data</td>
					<td>
						<?
						//if ($novo)
							echo campo_data( 'flcpubleditaldtprev', 'S', 'S', '', 'S' ); 
						//else
							//echo campo_data( 'flcpubleditaldtprev', 'S', 'N', '', 'S' );?>
					</td>
				</tr>
				<tr id="recurso" style="display: none;">
					<td align="center" colspan="2">
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td class="SubTituloDireita">Data</td>
								<td>
									<? #$flcdtrecintermotivo = $dadosObra['flcdata']; ?>
									<? 
									//if ($novo)
										echo campo_data( 'flcdtrecintermotivo', 'S', 'S', '', 'S' ); 
									//else
										//echo campo_data( 'flcdtrecintermotivo', 'S', 'N', '', 'S' );?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Motivo</td>
								<td>
									<? #$flcrecintermotivo = $dadosObra['flcrecintermotivo']; ?>
									<?= campo_textarea( 'flcrecintermotivo', 'N', 'S', '', '50', '4', '500'); ?>
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
									#$flcordservdt = $dadosObra['flcdata']; ?>
									<?
									//if ($novo)
										echo campo_data( 'flcordservdt', 'S', 'S', '', 'S' );
									//else
										//echo campo_data( 'flcordservdt', 'S', 'N', '', 'S' );
									 ?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Número da O.S.</td>
								<td>
									<?
									
									#$flcordservnum = $dadosObra['flcordservnum']; 
									?>
									<?= campo_texto( 'flcordservnum', 'N', 'S', '', 17, 15, '',  '', 'left', '', 0); ?>
								</td>
							</tr>
						</table>
					</td>
				</tr>
				<tr id="homologacao" style="display: none;">
					<td class="SubTituloDireita">Data</td>
					<td>
						<? #$flchomlicdtprev = $dadosObra['flchomlicdtprev'] ?>
						<?
						//if ($novo)
							echo campo_data( 'flchomlicdtprev', 'S', 'S', '', 'S' ); 
						//else
							//echo campo_data( 'flchomlicdtprev', 'S', 'N', '', 'S' )?>
					</td>
				</tr>
				<tr id="meio_publicacao" style="display: none;">
					<td class="SubTituloDireita">Meio de Publicação:</td>
					<td>
						<? #$flchomlicdtprev = $dadosObra['flchomlicdtprev'] ?>
						<?
						//if ($novo)
							echo campo_texto( 'flcmeiopublichomol', 'S', 'S', '', 80, 100, '',  '', 'left', '', 0);
						//else
							//echo campo_texto( 'flcmeiopublichomol', 'S', 'N', '', 80, 100, '',  '', 'left', '', 0);?>
						
					</td>
				</tr>
				<tr id="observacao" style="display: none;">
					<td class="SubTituloDireita">Observação:</td>
					<td>
						<? #$flchomlicdtprev = $dadosObra['flchomlicdtprev'] ?>
						<?
						//if ($novo)
							echo campo_textarea( 'flcobshomol', 'N', 'S', '', '84', '4', '500');
						//else
							//echo campo_textarea( 'flcobshomol', 'N', 'N', '', '84', '4', '500');?>
						
					</td>
				</tr>
				<tr id="abertura_de_proposta" style="display: none;">
					<td class="SubTituloDireita">Data</td>
					<td>
						<? #$flcaberpropdtprev = $dadosObra['flcaberpropdtprev']; ?>
						<?
						//if ($novo)
							echo campo_data( 'flcaberpropdtprev', 'S', 'S', '', 'S' );
						//else
							//echo campo_data( 'flcaberpropdtprev', 'S', 'N', '', 'S' ); ?>
					</td>
				</tr>
			</table>
			<input type=hidden name="data_form" id="data_form" value="">
			<input type=hidden name="data_opener" id="data_opener" value="">
			<?php 
			echo "<input type='hidden' name='flcid_f' id='flcid_f' value='".$_REQUEST["flcid"]."'/>";
			?>	
		<div id="anexos" style="display: none">
		<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />
		<input type="hidden" name="requisicao" id="requisicao" value="" />
		<input type="hidden" name="tpaid" id="tpaid" value="" />
		<table class="tabela" bgcolor="#f5f5f5" width="95%" cellspacing="1" cellpadding="3" align="center">
			<tr>
				<th colspan="2">Anexar Documentos</th>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
				<td><input type="file" name="arquivo" id="arquivo"/></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
				<td><?= campo_textarea( 'arqdescricao', 'N', 'S', '', 50, 2, 250 ); ?></td>
			</tr>
		</table>
	</form>
		<?
		$caminho_atual = '/obras/obras.php?modulo=principal/fases_licitacao&acao=A&requisicao=excluir';
		$permissaoBotaoExcluir = "'<center><a href=\"#\" onclick=\"javascript:ExcluirDocumento(\'" . $caminho_atual . "acao=A&requisicao=excluir" . "\',' || arq.arqid || ',' || aqb.aqoid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,";
		
		if( $_GET['tflid'] == 9 ) $tpaid = '(24)';
		else if( $_GET['tflid'] == 2 ) $tpaid = '(3)';
		else $tpaid = '(3, 24)';
		
		$sql = "SELECT
						{$permissaoBotaoExcluir}
						to_char(aqb.aqodtinclusao,'DD/MM/YYYY'),
						tarq.tpadesc,
						'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivo(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
						arq.arqtamanho || ' kbs' as tamanho ,
						arq.arqdescricao,								
						usu.usunome
					FROM
						((public.arquivo arq 
							INNER JOIN obras.arquivosobra aqb ON arq.arqid = aqb.arqid) 
							INNER JOIN obras.tipoarquivo tarq ON tarq.tpaid = aqb.tpaid) 
							INNER JOIN seguranca.usuario usu ON usu.usucpf = aqb.usucpf
					WHERE
						aqb.aqostatus = 'A' AND	aqb.obrid = '" . $_SESSION["obra"]["obrid"] . "'
						AND (arqtipo <> 'image/jpeg' AND arqtipo <> 'image/png' AND arqtipo <> 'image/gif')
						AND tarq.tpaid in $tpaid";
		
		$cabecalho = array( "Ação", 
							"Data Inclusão",
							"Tipo Arquivo",
							"Nome Arquivo",
							"Tamanho (Mb)",
							"Descrição Arquivo",
							"Responsável");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
	?>
	</div>
	<table class="tabela" bgcolor="#f5f5f5" width="95%" cellspacing="1" cellpadding="3" align="center">
		<tr bgcolor="#C0C0C0">
			<td colspan="2">
				<div style="float: left;">
					<input type="button" onclick="salvafases();" name="ok" value="Ok">
				</div>
			</td>
		</tr>
	</table>
	</body>
</html>
<div id="erro"></div>

<?php
	if($_REQUEST["tflid"]){
		echo "<script>";
			echo "abreCamposFaseLicitacao(".$_REQUEST["tflid"]."); ";
			echo " populaValoresCamposFaseLicitacao('".$_REQUEST["flcid"]."');";
		echo "</script>";
	}
	
?>