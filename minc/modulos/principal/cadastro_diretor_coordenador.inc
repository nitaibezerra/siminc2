<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js" language="javascript"></script>
<?php 
require_once APPRAIZ . 'includes/classes/entidades.class.inc';
// ver(1,2,3,$_SESSION['minc']);
if ($_REQUEST['opt'] == 'salvarRegistro') {
	 //REGRA DO SISTEMA, NÃO PERMITIR A MESMA PESSOA SER COORDENADOR E DIRETOR
	$tipo = array(FUN_DIRETOR_MC     => FUN_COORDENADOR_MC,
				  FUN_COORDENADOR_MC => FUN_DIRETOR_MC);
	
	if($tipo[$_REQUEST['funcoes']['funid']]) {
		if(existeDiretorCoordenadorPorCpf($tipo[$_REQUEST['funcoes']['funid']]) == str_replace(array(".","-"),"",$_REQUEST['entnumcpfcnpj'])) {
			echo "<script>
				alert('CPF ja cadastrado no perfil de ".$db->pegaUm("SELECT fundsc FROM entidade.funcao WHERE funid='".$tipo[$_REQUEST['funcoes']['funid']]."'")."');
				window.location='minc.php?modulo=principal/cadastro_diretor_coordenador&tipo=".$_REQUEST['tipo']."&acao=A';
			  </script>";
			exit;
		}
	} else {
		echo "<script>
				alert('Nenhuma função atribuida para este CPF');
				window.location='minc.php?modulo=principal/cadastro_diretor_coordenador&tipo=diretor&acao=A';
			  </script>";
		exit;
	}
	/*
	 * FIM
	 * REGRA DO SISTEMA, NÃO PERMITIR A MESMA PESSOA SER COORDENADOR E DIRETOR
	 */
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='minc.php?modulo=principal/cadastro_diretor_coordenador&tipo=".$_REQUEST['tipo']."&acao=A';
		  </script>";
	exit;
}

include_once APPRAIZ . 'includes/workflow.php';

# Verifica se existe entidade
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

if(!$_SESSION['minc']['mceid']){
	echo "<script>
			alert('Não existe escola atribuida para o seu Perfil.');
			window.location.href = '/minc/minc.php?modulo=inicio&acao=C'; 
		  </script>";
	die;
}

$somenteConsulta = false;
$boSalvar = (!$somenteConsulta) ? true : false;

// Tipo determina se é cadastro/consulta de Diretor ou Coordenador
$tipo = $_GET["tipo"];

if($tipo == "diretor"){
	$subtitulo = "Cadastro - Diretor";
	$funid = FUN_DIRETOR_MC;
	$stTipo = "Diretor";
} else {
	$subtitulo = "Cadastro - Coordenador";
	$funid = FUN_COORDENADOR_MC;
	$stTipo = "Coordenador";
}

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
echo monta_titulo($titulo, $subtitulo);
//if ($tipo == "coordenador"){
//	echo monta_titulo("", "Coordenador (a), ou professor comunitário, É a pessoa física, ligada ou não ao quadro de servidores da escola, que acompanhará todo o desenvolvimento do Plano de Atividade Cultural da Escola. <br>O coordenador (a) é escolhido em comum acordo entre a escola e iniciativa cultural parceira.");
//} 
echo cabecalho();
// ver($_SESSION['minc'],d);
/*
 * CÓDIGO DO NOVO COMPONENTE 
 */
$entidade = new Entidades();
$entidade->carregarPorFuncaoEntAssociado($funid,$_SESSION['minc']['entid']);

echo $entidade->formEntidade("minc.php?modulo=principal/cadastro_diretor_coordenador&tipo=$tipo&acao=A&opt=salvarRegistro",
							array("funid" => $funid, "entidassociado" => $_SESSION['minc']['entid']),
							array("enderecos"=>array(1),"obrigatorios"=>array("entnumcpfcnpj","entnome","entemail","entnumrg","entorgaoexpedidor","entsexo","endcep1",
							"entdatanasc","entnumresidencial")));

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

?>

<script type="text/javascript" language="javascript">
	<?php if($tipo == "coordenador"){ ?>
	jQuery("#tr_titulo").children().html('COORDENADOR (Professor Comunitário) <br /> É a pessoa física, ligada ou não ao quadro de servidores da escola, que acompanhará todo o desenvolvimento do Plano de Atividade Cultural da Escola. <br>O coordenador (a) é escolhido em comum acordo entre a escola e iniciativa cultural parceira.');
	<?php } ?>
	jQuery('#tr_entnumdddcomercial').hide();
	jQuery('#entnumdddcomercial').attr('disabled',true);
	jQuery('#entnumcomercial').attr('disabled',true);	
	jQuery('#entnumramalcomercial').attr('disabled',true);	
	jQuery('#tr_entnumdddfax').hide();
	jQuery('#entnumdddfax').attr('disabled',true);
	jQuery('#entnumfax').attr('disabled',true);	
	jQuery('#entnumramalfax').attr('disabled',true);	
	
	jQuery('#tr_entnumdddresidencial').find("[class='SubTituloDireita']").html("Telefone");
	jQuery('#funcoescadastradas').val('');
	jQuery('#btncancelar').val('Limpar');
	jQuery('#btncancelar').click( function(){
	var $inputs = jQuery('#frmEntidade :input');
	$inputs.each( function(){
			if( this.type != 'button' && this.type != 'submit' )
 				jQuery(this).val('');
 		});
	});
	
</script>

<?php
if( (count( $perfis ) == 1 && in_array(PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO, $perfis) ) || !$boSalvar ){
	echo "<script>
			document.getElementById('btngravar').disabled = 1;
		  </script> 
	";							 
}

?>
<script type="text/javascript">
<?
if($somenteConsulta) {
	echo "document.getElementById('btngravar').disabled=true;";
}
?>

jQuery(document).ready(function(){
        
        jQuery('#btngravar').attr('disabled', true);
        jQuery('#btncancelar').attr('disabled', true);
        
        <?php if(in_array(PERFIL_MINC_CADASTRADOR, $perfis) || in_array(PERFIL_MINC_SUPER_USUARIO, $perfis)){ ?>
            jQuery('#btngravar').attr('disabled', false);
            jQuery('#btncancelar').attr('disabled', false);
        <?php } ?>
            
});

function enviaForm(){
	if(jQuery('#entnumcpfcnpj').val() == ''){
		alert('CPF é obrigatório.');
    	return false;	
	}
	if(jQuery('#entnome').val() == ''){
		alert('O nome é obrigatório.');
    	return false;	
	}
	if(jQuery('#entemail').val() == ''){
		alert('O e-mail é obrigatório.');
    	return false;	
	}else{
		var x=jQuery('#entemail').val();
		var atpos=x.indexOf("@");
		var dotpos=x.lastIndexOf(".");
		if (atpos<1 || dotpos<atpos+2 || dotpos+2>=x.length)
		{
		  alert("Favor inserir um email válido.");
		  return false;
		}
	}
	if(jQuery('#entnumrg').val() == ''){
		alert('O RG é obrigatório.');
    	return false;	
	}
	if(jQuery('#entorgaoexpedidor').val() == ''){
		alert('O órgão expedidor é obrigatório.');
    	return false;	
	}
	if(jQuery('#entsexo').val() == ''){
		alert('O sexo é obrigatório.');
    	return false;	
	}
	if(jQuery('#entdatanasc').val() == ''){
		alert('A data de nascimento é obrigatório.');
		return false;
 	} 
    if(!validaData(document.getElementById('entdatanasc'))){
    	alert('Data de nascimento é inválida.');
    	return false;
    } 	
	if(jQuery('#entnumdddresidencial').val() == '' || jQuery('#entnumresidencial').val() == ''){
		alert('O telefone residencial é obrigatório.');
    	return false;
	}
	jQuery('#frmEntidade').submit();
}

function popupMapa(entid){
	//window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	window.open('http://culturaeduca.cc');
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