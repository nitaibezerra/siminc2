<?php
$_SESSION['minc']['entid'] = $_SESSION['minc']['entid'] ? $_SESSION['minc']['entid'] : $_GET['entid'];
$_SESSION['minc']['mceid'] = $_SESSION['minc']['mceid'] ? $_SESSION['minc']['mceid'] : $_GET['mceid'];


// Verifica se o 'memid' existe na sessão.
if(!$_SESSION['minc']['mceid'] || !$_SESSION['minc']['entid']) {
	echo "<script type=\"text/javascript\">
			alert(\"Sessão expirou. Entre novamente.\");
			location.href = \"minc.php?modulo=inicio&acao=C\";
		  </script>";
	exit;
}


require_once APPRAIZ . "includes/classes/entidades.class.inc";
define("FUNCAO_ESCOLA", 3);

if ($_REQUEST['opt'] == 'salvarRegistro') {
	/*
	 * CÓDIGO DO NOVO COMPONENTE 
	 */
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->salvar();
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='minc.php?modulo=principal/dados_escola&acao=A';
		  </script>";
	exit;
}

include_once APPRAIZ . 'includes/workflow.php';

$arPerfilMaisEducacaoVeLista = array( PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO,PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO);

$boExisteEntidade = boExisteEntidade( $_SESSION['minc']['entid'] );
if( !$boExisteEntidade ){
	echo "<script>
			alert('A Entidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

// Verifica se o 'entid' existe na sessão.
if(!$_SESSION['minc']['entid']) {
	echo "<script type=\"text/javascript\">
			alert(\"Ocorreu um erro com a entidade selecionada. Contate o Administrador do sistema.\");
			location.href = \"minc.php?modulo=inicio&acao=C\";
		  </script>";
	exit;
}

$somenteConsulta = false;
$boSalvar = (!$somenteConsulta) ? true : false;

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$sqlDocumento = "SELECT docid FROM minc.mcemaiscultura WHERE mceid = {$_SESSION['minc']['mceid']}";
$rsDocumento = !empty($_SESSION['minc']['mceid']) ? $db->pegaLinha($sqlDocumento) : array();

$docid = $rsDocumento['docid'];

$arMnuid = array();
if($docid){
    $sqlDocumento = "select * from workflow.documento where docid = {$docid}";
    $resultDocumento = $db->pegaLinha($sqlDocumento);

 	if ($resultDocumento && !in_array($resultDocumento['esdid'], array(/*ESTADO_DOCUMENTO_AVALIACAO, ESTADO_DOCUMENTO_FINALIZADO, ESTADO_DOCUMENTO_AVALIADO, */ESTADO_DOCUMENTO_ENVIADO_FNDE/*, ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR, ESTADO_DOCUMENTO_AVALIACAO*/))) {
        $arMnuid = array(MNUID_AVALIACAO, MNUID_MONITORAMENTO,MNUID_MONITORAMENTO2);
    }
}else{
	$arMnuid = array( MNUID_AVALIACAO, MNUID_MONITORAMENTO,MNUID_MONITORAMENTO2);
}

$db->cria_aba($abacod_tela, $url,'' , $arMnuid);

$titulo = "Mais Cultura nas Escolas";
$subtitulo = "Cadastro - Dados Escola";
echo monta_titulo($titulo, $subtitulo);
	
echo cabecalho();
// ver($_SESSION['minc'],$_POST,$_REQUEST,d);
/*
 * Código do componentes de entidade
 */
$entidade = new Entidades();

if($_SESSION['minc']['entid'])
	$entidade->carregarPorEntid($_SESSION['minc']['entid']);
	
?>

<table class="Tabela" align="center">
	<tr>
		<td align="center">A seta vermelha " <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">" indica que o campo é<strong> obrigatório</strong></td>
	</tr>
</table>
 
<?php 
echo $entidade->formEntidade("minc.php?modulo=principal/dados_escola&acao=A&opt=salvarRegistro",array("funid" => FUNCAO_ESCOLA, "entidassociado" => null),
array("enderecos"=>array(1),"obrigatorios"=>array("entcodent","entnome","entemail",
 "entnumcomercial","tpcid","tpsid","njuid","tplid"),"validaForm"=>"validaForm()"));

$perfis = arrayPerfil();


if($_SESSION['minc']['mceid']){
    
    // Validando inputs de acordo com o estado do documento
    $sqlDocumento = "SELECT docid
                                    FROM minc.mcemaiscultura 
                                    WHERE mceid = {$_SESSION['minc']['mceid']}";
    $rsDocumento = $db->pegaLinha($sqlDocumento);
    $docid = $rsDocumento['docid'];

    if($docid){
        $estado = wf_pegarEstadoAtual($docid);
//         ver($estado['esdid'],d);
        if(($estado['esdid'] != ESTADO_DOCUMENTO_CADASTRAMENTO) && ($estado['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR)){
            echo "<script>
                    setInterval(function(){
                            var inputs = document.getElementsByTagName('input');
                            var selects = document.getElementsByTagName('select');
                            var textareas = document.getElementsByTagName('textarea');

                            for(var i=0; i<textareas.length; i++){
                                textareas[i].disabled = true;
                             }

                            for(var i=0; i<selects.length; i++){
                                selects[i].disabled = true;
                             }

                            for(var i=0; i<inputs.length; i++){
                                inputs[i].disabled = true;
                             }

                            document.getElementById('btngravar').disabled = 1
                    },100);
                  </script>  
            ";
        }
    }
}
// VER($arMnuid,$estado['esdid']);
if( (count( $perfis ) == 1 && in_array(PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO, $perfis)) || !$boSalvar ){
	echo "<script>
			document.getElementById('btngravar').disabled = 1;
		  </script> 
	";							 
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js" language="javascript"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function(){
        
        jQuery('#btngravar').attr('disabled', true);
        jQuery('#btncancelar').attr('disabled', true);
        
        <?php if(in_array(PERFIL_MINC_CADASTRADOR, $perfis) || in_array(PERFIL_MINC_SUPER_USUARIO, $perfis)){ ?>
            jQuery('#btngravar').attr('disabled', false);
            jQuery('#btncancelar').attr('disabled', false);
        <?php } ?>
    
	<?php 
	if(in_array(PERFIL_MINC_ADMINISTRADOR, $perfis) || in_array(PERFIL_MINC_SUPER_USUARIO, $perfis)){
	?>
		jQuery('#tpcid').attr('disabled',false);
		jQuery('#tplid').attr('disabled',false);	
	<?php } else { ?>
		jQuery('#tpcid').attr('disabled',true);
		jQuery('#tplid').attr('disabled',true);	
	<?php
	}
	?>
	
	jQuery('#funcoescadastradas').val('');
	jQuery('#btncancelar').removeAttr('onclick');
	jQuery('#btncancelar').attr({value:'Limpar'});

	//Limpar o Formulário
	jQuery('#btncancelar').click( function(){
		jQuery('#frmEntidade')[0].reset(); 
	});
	
	jQuery('#tr_entnuninsest').hide();
	jQuery('#tr_entungcod').hide();
	jQuery('#tr_tpctgid').hide();
	jQuery('#tr_entunicod').hide();
	jQuery('#tr_njuid').hide();

	//verifica campo cnpj (HABILITANDO ou DESABILITANDO)
// 	if(jQuery('#entnumcpfcnpj').val()){
// 		jQuery('#entnumcpfcnpj').attr('readonly',true);
// 		jQuery('#entnumcpfcnpj').removeClass();
// 		jQuery('#entnumcpfcnpj').addClass('disabled');
// 		jQuery('#entnumcpfcnpj').unbind('blur');
// 	}
	//verifica campo Código da escola (INEP) (HABILITANDO ou DESABILITANDO)
	if(jQuery('#entcodent').val()){
		jQuery('#entcodent').attr('readonly',true);
		jQuery('#entcodent').removeClass();
		jQuery('#entcodent').addClass('disabled');
		jQuery('#entcodent').unbind('blur');
	}
	//verifica campo Nome (HABILITANDO ou DESABILITANDO)
	if(jQuery('#entnome').val()){
		jQuery('#entnome').attr('readonly',true);
		jQuery('#entnome').removeClass();
		jQuery('#entnome').addClass('disabled');
		jQuery('#entnome').unbind('blur');
	}
	//verifica campo Razão Social (HABILITANDO ou DESABILITANDO)
// 	if(jQuery('#entrazaosocial').val()){
// 		jQuery('#entrazaosocial').attr('readonly',true);
// 		jQuery('#entrazaosocial').removeClass();
// 		jQuery('#entrazaosocial').addClass('disabled');
// 		jQuery('#entrazaosocial').unbind('blur');
// 	}
	//verifica campo E-mail (HABILITANDO ou DESABILITANDO)
// 	if(jQuery('#entemail').val()){
// 		jQuery('#entemail').attr('readonly',true);
// 		jQuery('#entemail').removeClass();
// 		jQuery('#entemail').addClass('disabled');
// 		jQuery('#entemail').unbind('blur');
// 	}else{
// 		jQuery('#entemail').attr('readonly',false);
// 		jQuery('#entemail').removeClass();
// 		jQuery('#entemail').addClass('normal');
// 		jQuery('#entemail').unbind('blur');
// 	}
	jQuery('#entemail, #entnumcomercial, #entnumfax, #entnumdddcomercial, #entnumdddfax, #entnumramalcomercial, #entnumramalfax').removeClass();
	jQuery('#entemail, #entnumcomercial, #entnumfax, #entnumdddcomercial, #entnumdddfax, #entnumramalcomercial, #entnumramalfax').addClass('normal');
	
	//verifica campo Natureza Jurídica (HABILITANDO ou DESABILITANDO)
	if( jQuery('#njuid').val() ){
		jQuery('#njuid').attr('disabled',true);
		jQuery('#njuid').after('<input type="hidden" name="njuid" value="'+jQuery('#njuid').val()+'"/>');
	}
	//verifica campo Classificação (HABILITANDO ou DESABILITANDO)
	if(jQuery('#tpcid').val()){
		jQuery('#tpcid').attr('disabled',true);
		jQuery('#tpcid').after('<input type="hidden" name="tpcid" value="'+jQuery('#tpcid').val()+'"/>');
	}
	//verifica campo Localização (HABILITANDO ou DESABILITANDO)
	if(jQuery('#tplid').val()){
		jQuery('#tplid').attr('disabled',true);
		jQuery('#tplid').after('<input type="hidden" name="tplid" value="'+jQuery('#tplid').val()+'"/>');
	}
	//verifica campo Situação (HABILITANDO ou DESABILITANDO)
	if(jQuery('#tpsid').val()){
		jQuery('#tpsid').attr('disabled',true);
		jQuery('#tpsid').after('<input type="hidden" name="tpsid" value="'+jQuery('#tpsid').val()+'"/>');
	}

// 	jQuery('#entnumcpfcnpj').remove();
	jQuery('#tr_entnumcpfcnpj, #tr_entrazaosocial').remove();
});

function enviaForm(){
// 	if(jQuery('#entnumcpfcnpj').val() == ''){
// 		alert('Informe o CNPJ.');
//     	return false;
// 	}
	
// 	var cnpj2 = jQuery('#entnumcpfcnpj').val();
// 	if(!validarCnpj2(cnpj2)){
// 		alert('CNPJ inválido.');
//     	return false;
// 	}
	
	if(jQuery('#entcodent').val() == ''){
		alert('Informe o Código da escola (INEP).');
    	return false;
	}	
	if(jQuery('#entnome').val() == ''){
		alert('O nome da entidade é obrigatório.');
		return false;
	}	
// 	if(jQuery('#entrazaosocial').val() == ''){
// 		alert('A razão social é obrigatória.');
// 		return false;
// 	}
	if(jQuery('#entemail').val() == ''){
		alert('O e-mail é obrigatório.');
		return false;
	}

	var email = jQuery('#entemail').val();
    var emailValido=/^.+@.+\..{2,}$/;
    if(!emailValido.test(email))
    {
	      alert('O e-mail é inválido. Por Favor, verifique!');
	      return false;
    }

	if(jQuery('#entnumdddcomercial').val() == '' || jQuery('#entnumcomercial').val() == ''){
		alert('O telefone comercial é obrigatório.');
		return false;
	}

	var ddd = jQuery('#entnumdddcomercial').val();
	if(ddd.length < 2){
		alert('O DDD do telefone comercial é inválido.');
		return false;
	}
	
	var fone = jQuery('#entnumcomercial').val();	
	if(fone.length < 9){
		alert('O telefone comercial é inválido.');
		return false;
	}
	
// 	if( jQuery('#njuid').val() == ''){
// 		alert('A Natureza Jurídica é obrigatória.');
// 		return false;
// 	}
	if(jQuery('#tpcid').val() == ''){
		alert('A classifcação é obrigatória.');
		return false;
	}		
	if(jQuery('#tplid').val() == ''){
		alert('A Localização é obrigatória.');
		return false;
	}		
	if(jQuery('#tpsid').val() == ''){
		alert('A situação é obrigatória.');
		return false;
	}		
	if(jQuery('#endcep1').val() == ''){
		alert('O CEP é obrigatório.');
		return false;
	}	
	if(jQuery('#endlog1').val() == ''){
		alert('O Logradouro é obrigatório.');
		return false;
	}
	if(jQuery('#endnum1').val() == ''){
		alert('O Número é obrigatório.');
		return false;
	}
	if(jQuery('#endbai1').val() == ''){
		alert('O Bairro é obrigatório.');
		return false;
	}
	if(jQuery('#estuf1').val() == ''){
		alert('UF inválido. Digite novamente o CEP.');
		return false;
	}
	if(jQuery('#mundescricao1').val() == ''){
		alert('Munícipio inválido. Digite novamente o CEP.');
		return false;
	}
	jQuery('#frmEntidade').submit();
}

setTimeout(function() {
	document.getElementById('entnumcomercial').value = mascaraglobal('####-####',document.getElementById('entnumcomercial').value)
	document.getElementById('entnumfax').value = mascaraglobal('####-####',document.getElementById('entnumfax').value)

}, 1000);


function popupMapa(entid){
	//window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	window.open('http://culturaeduca.cc');
}

function validarCnpj2(cnpj) {
	cnpj = cnpj.replace(/[^0-9]/ig, '');
    if (cnpj.length != 14)
        return false;

    if (cnpj.length != 14)
        return false;

    var dv      = false;
    var cnpj_dv = cnpj.substr( 12, 2 );
    
    var digito  = 0;
    var controle = '';

    for (var i = 0; i < 2; i++ ) {
        var soma = 0;
        for ( var j = 0; j < 12; j++ )
            soma += cnpj.substr(j, 1) * ((11 + i - j) % 8 + 2);

        if ( i == 1 )
            soma += digito * 2;

        digito = 11 - soma  % 11;

        if ( digito > 9 )
            digito = 0;

        controle += digito + '';
    }

    return controle == cnpj_dv;
}

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow

function displayMessage(url){

	messageObj.setSource(url);
	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(450,200);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function displayStaticMessage(messageContent,cssClass){
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(400,250);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage(){
	messageObj.close();	
}
</script>

<?php if(verificaEncerramentoPlanoEscola()): ?>

	<script>
		jQuery(function(){			
			jQuery.each(jQuery('#frmEntidade')[0].elements, function(i, v){
				jQuery(v).attr('disabled', true).removeClass('normal').addClass('disabled');
			});
			jQuery('#btncancelar, #btngravar, #tr_acoes').remove();
		});
	</script>

<?php endif; ?>