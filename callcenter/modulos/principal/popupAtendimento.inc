<?php
$programaAtendimento = $_REQUEST['sisid'];
validaSession($_SESSION['callcenter']['temid']);
include_once( APPRAIZ."callcenter/classes/Ligacao.class.inc");

$obLigacao = new Ligacao();
//$obTema = new Tema();
/******************************* SALVA NOVA LIGAÇÃO ************************************/
if($_POST['requisicao'] == 'salvar'){	
	
	if( empty($_REQUEST['ligid']) ){		
		$ligid = $obLigacao->insereLigacao( $_POST );
		
		switch ($_SESSION['callcenter']['temid']) {
			case 1:
				if( !empty($ligid) ){
					echo "<script> 
							alert( 'Operação realizada com sucesso. ' );
								window.opener.salvarAtendimento('".$ligid."', '".$_REQUEST['proid']."');
					  	  </script>";
				} else {
					echo "<script> 
								alert( 'Falha na operação. ' );
								parent.closeItensNew();
					  	  </script>";
				}
			break;
		}
	} else {
		if( $obLigacao->alteraLigacao($_POST) ){
			echo "<script> 
					alert( 'Operação realizada com sucesso. ' );
						window.close();
			  	  </script>";
			exit();
		} else {
			echo "<script> 
						alert( 'Falha na operação. ' );
			  	  </script>";
		}
	}
} else if( $_REQUEST['requisicao'] == 'alterar' && $_GET['ligid'] ){
	
	$arLigacao = $obLigacao->carregaDadosLigacao( $_GET['ligid'] );
	extract($arLigacao);
	
	$ligdataprovidenciamec = formata_data( $ligdataprovidenciamec );
	$ligdataprovidenciacontato = formata_data($ligdataprovidenciacontato);
}
				


?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
	    <title>Atendimento</title>
	    
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script src="../includes/funcoes.js"> </script>
	    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	    
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	    <script>
	    function abrirObrasPac2(proid) {
	    	window.open('callcenter.php?modulo=principal/listaObrasPac2&acao=A&proid='+proid,'Observações','scrollbars=yes,height=400,width=800,status=no,toolbar=no,menubar=no,location=no');
	    }
	    </script>
	</head>
	<body>
		<form name="formulario" id="formulario" method="post" action="" >
			<input type="hidden" name="requisicao" id="requisicao" value="salvar"  />
			<input type="hidden" name="temid" id="temid" value="<?=$_GET['temid']; ?>"  />
			<input type="hidden" name="proid" id="proid" value="<?=$_GET['proid']; ?>"  />
			<input type="hidden" name="programa" value="<?php echo $programaAtendimento; ?>"  />
			<input type="hidden" name="tlgid" id="tlgid" value="<?=$_GET['tlgid']; ?>" />
			<input type="hidden" name="ligid" id="ligid" value="<?=$_GET['ligid']; ?>" />
			<table align="center" border="0" cellpadding="3" cellspacing="1" class="tabela">
				<tr>
					<td colspan="3" class="SubTituloCentro">Atendimento ao Cliente</td>
				</tr>
				<!--<tr>
					<td class="SubTituloDireita">Tipo de Ligação:</td>
					<td>
						<?php
						/*$arLigacao = array( "Efetivado" => array("valor" => "1", "id" => "tlgid_1", 'default' => 'S'),
										 	"Recebida"  => array("valor" => "2", "id" => "tlgid_2")
										);
						echo campo_radio( 'tlgid', $arLigacao, 'h'); echo obrigatorio();*/
						?>
					</td>
				</tr>-->
				<tr>
					<td class="SubTituloDireita">Telefone de Contato:</td>
					<td>
						<?php echo campo_texto( "ligdddtelefone", 'N', 'S', '', '2', '2', '', '', '', '', '', 'id="ligdddtelefone"', '', '' ) ." - ". campo_texto( "lignumerotelefone", 'S', 'S', '', '14', '14', '', '', '', '', '', 'id="lignumerotelefone"', '', '' );  ?>
					</td>
					<td><a style="cursor:pointer;" onclick="abrirObrasPac2('<?=$_REQUEST['proid'] ?>');">Abrir informações sobre o município</a></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Nome do Contato:</td>
					<td colspan="2">
						<?php echo campo_texto( "lignomecontato", 'S', 'S', '', '20', '30', '', '', '', '', '', 'id="lignomecontato"', '', '' ); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Cargo do Contato:</td>
					<td colspan="2">
						<?php echo campo_texto( "ligcargocontato", 'S', 'S', '', '20', '30', '', '', '', '', '', 'id="ligcargocontato"', '', '' ); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Dados da Conversa:</td>
					<td colspan="2">
						<?php echo campo_textarea("ligtexto", 'S', '', '', 80, 5, 0 ,'','','','','', '' /*$obLigacao->providenciamec*/); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Providência do Contato:</td>
					<td colspan="2">
						<?php echo campo_textarea("ligprovidenciacontato", 'N', '', '', 80, 5, 0 ,'','','','','', '' /*$obLigacao->providenciamec*/); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Data da providência do Contato:</td>
					<td colspan="2">
						<?php echo campo_data2('ligdataprovidenciacontato', 'N','S','Data da providência do Contato','','',''); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Providência MEC:</td>
					<td colspan="2">
						<?php echo campo_textarea("ligprovidenciamec", 'N', '', '', 80, 5, 0 ,'','','','','', '' /*$obLigacao->providenciamec*/); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Data da providência do MEC:</td>
					<td colspan="2">
						<?php echo campo_data2('ligdataprovidenciamec', 'N','S','Data da providência do MEC','','',''); ?>
					</td>
				</tr>
				<tr>
					<td colspan="3" style="text-align: center; background-color: #E9E9E9;" ><input type="button" onclick="salvarAtedimento();" value="Finalizar Atendimento" /> </td>
				</tr>
			</table>
		</form>
	</body>
	<?
	echo '<br>';
	$obLigacao->montaListaLigacao( $_REQUEST );
	?>
	<script type="text/javascript">
		function alterarLigacao(ligid){
			var tlgid = window.document.getElementById('tlgid').value;
			var proid = window.document.getElementById('proid').value;
			window.location.href = 'callcenter.php?modulo=principal/popupAtendimento&acao=A&proid='+proid+'&tlgid='+tlgid+'&ligid='+ligid+'&requisicao=alterar';
		}
		function mostraLigacao(ligid){
			var proid = window.document.getElementById('proid').value;
			window.open('callcenter.php?modulo=principal/popupMostraAtendimento&acao=A&ligid='+ligid+'&proid='+proid,'mostraLigacoes','scrollbars=yes,height=500,width=800,status=no,toolbar=no,menubar=no,location=no');
		}
		
		function salvarAtedimento(){
			var erro = 0;
			$("[class~=obrigatorio]").each(function() { 
				if(!this.value || this.value == "Selecione..."){
					erro = 1;
					alert('Favor preencher todos os campos obrigatórios!');
					this.focus();
					return false;
				}
			});
			if(erro == 0){;
				document.getElementById('requisicao').value = 'salvar';
				$("#formulario").submit();
			}
		}
		
	</script>
</html>