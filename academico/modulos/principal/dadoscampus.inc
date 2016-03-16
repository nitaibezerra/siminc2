<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

    jQuery.noConflict();

    jQuery(document).ready(function(){
        //Desativar enter no componente entidade. Feito dessa forma pois o IE não deixa alterar o tipo de um input
        var value = jQuery('#btngravar').attr('value'); // grab value of original
        var html = '<input type="button" id="btngravar" name="btngravar" value="'+value+'" />';
        jQuery('#btngravar').after(html).remove(); // add new, then remove original input

        //VALIDA CAMPOS OBRIGATÓRIOS.
        jQuery('#btngravar').click(function(){
            
            var entnome = jQuery('#entnome');
            var endcep1 = jQuery('#endcep1');
            var erro;
            
            if(jQuery('#entnome').val() == ""){
                alert('O campo "Nome do Campo" é um campo obrigatório!');
                entnome.focus();
                erro = 1;
                return false;
            }
            
            if(jQuery('#endcep1').val() == ""){
                alert('O campo "CEP" é um campo obrigatório!');
                endcep1.focus();
                erro = 1;
                return false;
            }
            
            if(!erro){
                jQuery('#frmEntidade').submit();
            }
        });        
    });

</script>

<?PHP
require_once APPRAIZ . "includes/classes/entidades.class.inc";

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$_SESSION['academico']['entidcampus'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entidcampus'];
$entidcampus = $_SESSION['academico']['entidcampus'] ? $_SESSION['academico']['entidcampus'] : $_REQUEST['entid'];

$existecampus = academico_existecampus( $entidcampus );

if( !$existecampus ){
    echo "
        <script>
            alert('O Campus informado não existe!');
            history.back(-1);
        </script>
    ";
    die;
}

if( academico_existefuncao($existecampus, FUNCAO_REITORIA) ){
    $funid = FUNCAO_REITORIA;
    $titulo = 'Dados da Reitoria';
} else {
    $funid = FUNCAO_CAMPUS;
    $titulo = 'Dados do Campus';
}

$perfilNotBloq = array(
        PERFIL_IFESCADTCU,
        PERFIL_MECCADBOLSAS,
        PERFIL_MECCADCURSOS,
        PERFIL_MECCADASTRO,
        PERFIL_ADMINISTRADOR,
        PERFIL_CADASTROGERAL,
        PERFIL_REITOR,
        PERFIL_INTERLOCUTOR_INSTITUTO,
        PERFIL_INTERLOCUTOR_CAMPUS,
        PERFIL_DIRETOR_CAMPUS,
        PERFIL_PROREITOR
);

// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
$permissoes['gravar'] = academico_possui_perfil(array(PERFIL_ADMINISTRADOR,PERFIL_REITOR,PERFIL_INTERLOCUTOR_INSTITUTO,PERFIL_INTERLOCUTOR_CAMPUS,PERFIL_DIRETOR_CAMPUS,PERFIL_PROREITOR)) ? true : false;
validaAcessoTipoEnsino($permissoes['vertipoensino'], $_SESSION['academico']['orgid']);
// Fim segurança

if($_REQUEST['opt'] && $_REQUEST['entid'] && $permissoes['gravar'] ){
    if($_REQUEST['opt'] == 'salvarRegistro') {
        $entidade = new Entidades();
        $entidade->carregarEntidade($_REQUEST);
        $entidade->salvar();

    	if( $_REQUEST['tipoEntidade'] == 'reitoria' ){
            $modulo = 'alterarReitoria';
        } else {
            $modulo = 'alterarCampus';
        }

        echo "
            <script>
                alert('Dados gravados com sucesso');
        	window.location='?modulo=principal/$modulo&acao=A&entid=".$_REQUEST['entid']."';
            </script>
        ";
        exit;
    }
}
$bloqueado = $permissoes['gravar'] == true ? true : false ;

// Busca a unidade pai do Campus
$autoriazacaoconcursos = new autoriazacaoconcursos();
$_SESSION['academico']['entid'] = $autoriazacaoconcursos->buscaentidade($entidcampus, $tipoEntidade);

if($entidcampus){
    $_SESSION['academico']['entidadenivel'] = 'campus';
}
// Monta Abas
if( $funid == FUNCAO_CAMPUS ){
    if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
            $abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57277' : '57136'; // Dados Desenv
    }else{
            $abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57268' : '57136';
    }
}elseif( $funid == FUNCAO_REITORIA ){
    if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
            $abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57294' : '57270'; // Dados Desenv
    }else{
            $abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57273' : '57263';
    }
}

#$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo, "");
?>

<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
<?php
// cria o cabeçalho padrão do módulo
echo $autoriazacaoconcursos->cabecalho_entidade($entidcampus);

$sql_entid = "
    SELECT  fea.entid
    FROM entidade.entidade ent

    LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
    LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid

    WHERE fen.funid = {$funid} AND fen.entid = {$entidcampus}
";
$entidassociado = $db->pegaUm($sql_entid);

$entidade = new Entidades();
$entidade->carregarPorEntid($entidcampus);


echo $entidade->formEntidade(
    "academico.php?modulo=principal/dadoscampus&acao=A&opt=salvarRegistro&tipoEntidade=$tipoEntidade",
    array("funid" => $funid, "entidassociado" => $entidassociado),
    array("enderecos"=>array(1), "fotos"=>true, "gravar"=>$permissoes['gravar'] )
);
?>
</div>
<?php
$perfis = pegaArrayPerfil( $_SESSION['usucpf'] );

?>
<script type="text/javascript">
    <?PHP
        #Desabilitando no caso de estar bloqueado
        if( $bloqueado == false ) {
           echo "document.getElementById('btngravar').disabled=true";
        }
    ?>

    document.getElementById('tr_entcodent').style.display           = 'none';
    document.getElementById('tr_entnuninsest').style.display        = 'none';
    document.getElementById('tr_entungcod').style.display           = 'none';
    document.getElementById('tr_tpctgid').style.display             = 'none';
    document.getElementById('tr_entnumcpfcnpj').style.display       = 'none';
    document.getElementById('tr_njuid').style.display               = 'none';
    document.getElementById('tr_tpcid').style.display               = 'none';
    document.getElementById('tr_tplid').style.display               = 'none';
    document.getElementById('tr_tpsid').style.display               = 'none';
    document.getElementById('tr_funcoescadastradas').style.display  = 'none';
    
    
    document.getElementById('entemail').className  = 'normal';
    document.getElementById('entsig').className  = 'normal';
    
    document.getElementById('entnumdddcomercial').className  = 'normal';
    document.getElementById('entnumcomercial').className  = 'normal';
    document.getElementById('entnumramalcomercial').className  = 'normal';
    
    document.getElementById('entnumdddfax').className  = 'normal';
    document.getElementById('entnumfax').className  = 'normal';
    document.getElementById('entnumramalfax').className  = 'normal';
    
    
    document.getElementById('endcep1').className  = 'obrigatorio normal';
    jQuery('#endcep1').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
    document.getElementById('endcom1').className  = 'normal';
    document.getElementById('endnum1').className  = 'normal';
    
    document.getElementById('btngravar').setAttribute('onclick', 'validarCamposEnd()');
    
    
    
    

<?PHP
    if ( in_array(PERFIL_ADMINISTRADOR, $perfis) || in_array(PERFIL_SUPERUSUARIO, $perfis) || in_array(PERFIL_CADASTROGERAL, $perfis)  ){
?>
    /*
     * DESABILITANDO O CAMPO DE CNPJ
     */
    document.getElementById('entnumcpfcnpj').readOnly = true;
    document.getElementById('entnumcpfcnpj').className = 'disabled';
    document.getElementById('entnumcpfcnpj').setAttribute('onfocus', '');
    document.getElementById('entnumcpfcnpj').setAttribute('onmouseout', '');
    document.getElementById('entnumcpfcnpj').setAttribute('onblur', '');
    document.getElementById('entnumcpfcnpj').setAttribute('onkeyup', '');

    /*
     * DESABILITANDO A SIGLA DA ENTIDADE
     */
    /*
    document.getElementById('entsig').readOnly = true;
    document.getElementById('entsig').className = 'disabled';
    document.getElementById('entsig').setAttribute('onfocus', '');
    document.getElementById('entsig').setAttribute('onmouseout', '');
    document.getElementById('entsig').setAttribute('onblur', '');
    document.getElementById('entsig').setAttribute('onkeyup', '');
*/
    /*
     * DESABILITANDO O NOME DA ENTIDADE
     */
    document.getElementById('entnome').readOnly = true;
    document.getElementById('entnome').className = 'disabled';
    document.getElementById('entnome').setAttribute('onfocus', '');
    document.getElementById('entnome').setAttribute('onmouseout', '');
    document.getElementById('entnome').setAttribute('onblur', '');
    document.getElementById('entnome').setAttribute('onkeyup', '');

<?PHP
    }else{
?>

    document.getElementById('entnumcpfcnpj').setAttribute('onblur', '');
    document.getElementById('entnome').setAttribute('onfocus', '');
    document.getElementById('entnome').setAttribute('onmouseout', '');
    document.getElementById('entnome').setAttribute('onblur', '');
    document.getElementById('entnome').setAttribute('onkeyup', '');

<?PHP
    }
?>


<?PHP
    if ( in_array(PERFIL_ADMINISTRADOR, $perfis) || in_array(PERFIL_SUPERUSUARIO, $perfis)  || in_array(PERFIL_CADASTROGERAL, $perfis)  ){
?>
    /*
     * Habilitando o campo NOME
     */
    var entnome = document.getElementById('entnome');
    entnome.readOnly = false;
    entnome.className = 'normal';
    entnome.setAttribute('onfocus', 'MouseClick(this);this.select();');
    entnome.setAttribute('onmouseout', 'MouseOut(this);');
    entnome.setAttribute('onblur', 'MouseBlur(this);');

    /*
     * Habilitando o campo Razão Social
     */
    var entnome = document.getElementById('entrazaosocial');
    entnome.readOnly = false;
    entnome.className = 'normal';
    entnome.setAttribute('onfocus', 'MouseClick(this);this.select();');
    entnome.setAttribute('onmouseout', 'MouseOut(this);');
    entnome.setAttribute('onblur', 'MouseBlur(this);');
    //document.getElementById('entnumcpfcnpj').setAttribute('onkeyup', 	'');
    jQuery('#entnome').after('&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
<?PHP
    }else{
?>
    /*
     * DESABILITANDO O CÓDIGO DA UNIDADE
     */
    document.getElementById('entunicod').readOnly = true;
    document.getElementById('entunicod').className = 'disabled';
    document.getElementById('entunicod').setAttribute('onfocus', '');
    document.getElementById('entunicod').setAttribute('onmouseout', '');
    document.getElementById('entunicod').setAttribute('onblur', '');
    document.getElementById('entunicod').setAttribute('onkeyup', '');

<?PHP
    }
?>
</script>

<?php
    //if ( in_array(PERFIL_ASSISTENCIA_ESTUDANTIL, $perfis) ){
    if( !academico_possui_perfil(array(PERFIL_SUPERUSUARIO,PERFIL_ADMINISTRADOR,PERFIL_REITOR,PERFIL_DIRETOR_CAMPUS,PERFIL_INTERLOCUTOR_CAMPUS,PERFIL_INTERLOCUTOR_INSTITUTO, PERFIL_PROREITOR)) ){
?>
	<script>
            jQuery('input[type=button]').remove();
	</script>
<?php
    } 
?>