<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );

$obrid = $_SESSION['obras2']['obrid'];

$declaracaoConformidade = new DeclaracaoConformidade();
$declaracaoConformidade->carregaPorObrid( $obrid );
switch ( $_POST['op'] ){
	case 'salvar':
		$obraLic = new ObraLicitacao();
		
		$arDado = $_POST;
		$arDado['oblid']  = $obraLic->pegaOblidPorObrid( $obrid );
		$arDado['usucpf'] = $_SESSION['usucpf'];
		$declaracaoConformidade->popularDadosObjeto( $arDado )
							   ->salvar();		
		$db->commit();					   
		$db->sucesso('principal/cadDeclaracaoConformidade');
}

switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=ISO-8859-1');

		$municipio = new Municipio();
		echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
		exit;		
}

extract( $declaracaoConformidade->getDados() );
$dadoUser = $declaracaoConformidade->dadosUsuarioCadastrante( $dlcid );

if( possui_perfil(PFLCOD_CALL_CENTER) ){
    $desabilita = "disabled = 'disabled'";
    $hab        = 'N';
}else{
    $hab        = 'S';
    $desabilita = '';
    
}
?>
<html>
<head>
	<title>Declaração de Conformidade</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	<script type="text/javascript">
	function carregarMunicipio( estuf ){
		var td	= $('#td_municipio');
		if ( estuf != '' ){
			var url = location.href;
			$.ajax({
				  url  		 : url,
				  type 		 : 'post',
				  data 		 : {ajax  : 'municipio', 
				  		  	    estuf : estuf},
				  dataType   : "html",
				  async		 : false,
				  beforeSend : function (){
				  	divCarregando();
					td.find('select option:first').attr('selected', true);
				  },
				  error 	 : function (){
				  	divCarregado();
				  },
				  success	 : function ( data ){
				  	td.html( data );
				  	divCarregado();
				  }
			});	
		}else{
			td.find('select option:first').attr('selected', true);
			td.find('select').attr('selected', true)
							 .attr('disabled', true);
		}			
	}
	
	function validaForm(){
		var msg = '';
		
		if ($('#estuf').val() == ''){
			msg += 'UF \n';
		}	
		
		if ($('#muncod').val() == ''){
			msg += 'Município \n';
		}	
		
		if ($.trim( $('[name=dlcnumero]').val() ) == ''){
			msg += 'Número do termo de compromisso \n';
		}	
		
		if (msg){
			msg = 'Os seguintes campos devem ser preenchidos:\n\n' + msg;
			alert(msg);
			return false;
		}
		
		$('#form_declaracao').submit();
	}
	</script>
</head>
<body topmargin="0" leftmargin="0">
<?php 
monta_titulo( "Declaração de Conformidade com a Lei 8.666/93", "" );
?>
<form method="post" action="" name="form_declaracao" id="form_declaracao" enctype="multipart/form-data">
<input type="hidden" name="op" id="op" value="salvar">
<table class="Tabela" align="center">
	<tr>
		<td class="SubTituloDireita">UF:</td>
		<td>
		<?php
		$uf = new Estado();					
		$db->monta_combo("estuf", $uf->listaCombo(), $hab,'Selecione...','carregarMunicipio', '', '',200,'S','estuf'); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Município:</td>
		<td id="td_municipio">
		<?php 
		if ($estuf && !possui_perfil(PFLCOD_CALL_CENTER)){
			$municipio = new Municipio( $muncod );
			$dado 	   = $municipio->listaCombo( array('estuf' => $estuf) );	
			$habMun    = 'S';
		}else{
			$dado   = array();
			$habMun = 'N';
		}		
		$habMun = ($disable == 'N' ? $disable : $habMun);
		echo $db->monta_combo("muncod", $dado, $habMun,'Selecione...','', '', '',200,'S','muncod'); 
		?>
		</td>
	</tr>
	<tr>
	    <td class="SubTituloDireita">Número do termo de compromisso:</td>
	    <td>
			<?= campo_texto( 'dlcnumero', 'S', $hab, '', 20, 20, '[#]', '', 'right', '', 0, '', ''); ?>    
		</td>
	</tr>
  	<tr style="" id="trButton" bgcolor="#C0C0C0">
  		<td colspan="2" align="center">
  			<input type="button" <?php echo $desabilita;?> value="Salvar" onclick="validaForm();">
  		</td>
  	</tr>
<?php 
	if ( $dlcid ):
?>  	
	<tr>
	    <td class="SubTituloDireita" colspan="2">
	    &nbsp;
		</td>
	</tr>
	<tr>
	    <td colspan="2">
		DECLARAÇÃO
		<br>
		<br>
		Declaro que o Estado / Município de <?php echo $estuf ?>/<?php echo $municipio->mundescricao ?>  cumpriu todas as determinações constantes na Lei nº 8.666, de 1993, bem como os demais dispositivos legais aplicáveis ao procedimento licitatório, ao executar as obras contempladas por meio do Termo de Compromisso nº <?php echo $dlcnumero ?>.
		<br>
		<br>
		Assinatura eletrônica	    
		<br>
		<?php
		echo $dadoUser['usucpf'] . ' - ' . $dadoUser['usunome'] 	    
		?>
		</td>
	</tr>
<?php
	endif;
?>	
</table>
</form>
</body>
</html>

