<?php
error_reporting(1);
error_reporting(E_ALL ^ E_NOTICE);

// set_include_path('.;D:\Workspace\php\pdeinterativo\includes;D:\Workspace\php\pdeinterativo\global;');
// $_SESSION['usucpforigem'] = '';
// $_SESSION['usucpf'] = '';
// $_SESSION['superuser'] = '1';

include "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once "classes/Encode.class.inc";
include_once "classes/Item.class.inc";
include_once "classes/Participante.class.inc";
include_once "classes/Questionario.class.inc";
include_once "classes/Meta.class.inc";
include_once "classes/Escolaridade.class.inc";
include_once "classes/Atuacao.class.inc";
include_once "classes/InstituicaoTipo.class.inc";
include_once "classes/SubMeta.class.inc";
include_once "classes/Avaliacao.class.inc";
include_once "classes/AvaliacaoOpcao.class.inc";
include_once "classes/Comentario.class.inc";
include_once "classes/Estado.class.inc";
include_once "classes/Municipio.class.inc";

/**
 * Comment: Criação de Classes
 * Author:  Rafael Freitas Carneiro
 * Date:    01/10/2015
 */
$item = new Item();
$avaliacaoC = new Avaliacao();
$atuacaoC = new Atuacao();
$escolaridadeC = new Escolaridade();
$instituicaoTipoC = new InstituicaoTipo();
$avaliacaoOpcao = new AvaliacaoOpcao();
$comentario = new Comentario();
$meta = new Meta();
$subMeta = new SubMeta();
$questionario = new Questionario($_SESSION['queid_pne']);
$particiante = new Participante($_SESSION['parid_pne']);
$municipio = new Municipio();
$estado = new Estado();
//Fim Criação Classes


/**
 * Carregando objetos
 * Author:  Rafael Freitas Carneiro
 * Date:    01/10/2015* 
 */
$metas = $meta->recuperarTodos('*', null, 'metid');
$instituicaoTipo = $instituicaoTipoC->recuperarTodos();
$avaliacaoOpcoes = $avaliacaoOpcao->recuperarTodos('*', null, 'avocodigo desc');
$metid = isset($_REQUEST['metid']) ? $_REQUEST['metid'] : $metas[0]['metid'];
for($i=0;$i<count($metas);$i++){
    if ($metas[$i]['metid']==$metid){        
        $mettitulo = $metas[$i]['mettitulo'];        
    }
}
for($i=0;$i<=count($metas);$i++){
    if ($metas[$i]['metid']==$metid){
        if ($i<count($metas)){
            $proximaMeta = $metas[$i+1]['metid'];
        }else{
            $proximaMeta = '';
        }
        if ($i==0){
            $anteriorMeta = '';
        }else{
            $anteriorMeta = $metas[$i-1]['metid'];
        }        
    } 
}
$subMetas = $subMeta->carregaSubMetasPorMeta($metid);
$estados = $estado->recuperarTodos('estuf, estdescricao', null, 'estdescricao');
$municipios = $particiante->estuf ? $municipio->recuperarTodos('muncod, mundescricao', array("estuf = '{$particiante->estuf}'"), 'mundescricao') : array();
$escolaridade = $escolaridadeC->recuperarTodos('*', null, 'escordem');
$atuacao = $atuacaoC->recuperarTodos('*', null, 'atuordem');
//Fim Carregando Objetos

/*
 * Cálculo da Idade do Participante
 * Author:  Rafael Freitas Carneiro
 * Date:    01/10/2015* 
 */
//$date = new DateTime( $particiante->pardatanascimento ); // data de nascimento
//$interval = $date->diff( new DateTime( date('Y-m-d') ) ); // data definida
//$idade = $interval->format( '%Y Anos' );
//

if ($particiante->parsexo=='M'){
    $sexo = 'Masculino';
}else if ($particiante->parsexo=='F'){
    $sexo = 'Feminino';
}else{
    $sexo = 'Outro';
}

if (! function_exists('array_column')) {
    function array_column(array $input, $columnKey, $indexKey = null) {
        $array = array();
        foreach ($input as $value) {
            if ( ! isset($value[$columnKey])) {
                trigger_error("Key \"$columnKey\" does not exist in array");
                return false;
            }
            if (is_null($indexKey)) {
                $array[] = $value[$columnKey];
            }
            else {
                if ( ! isset($value[$indexKey])) {
                    trigger_error("Key \"$indexKey\" does not exist in array");
                    return false;
                }
                if ( ! is_scalar($value[$indexKey])) {
                    trigger_error("Key \"$indexKey\" does not contain scalar value");
                    return false;
                }
                $array[$value[$indexKey]] = $value[$columnKey];
            }
        }
        return $array;
    }
}
?>
<!DOCTYPE html>
<html>

<?php require "head.php"; ?>


<body>

    <header class="navbar-fixed-top header" >
        <div class="row">
            <div class="col-lg-12 col-sm-12 col-xs-12">
                <img src="imagens/logo-simec.png" class="res" width="150">
                <a class="brasil pull-right" href="http://www.brasil.gov.br/"><img alt="Brasil - Governo Federal" src="/estrutura/temas/default/img/brasil.png" style="margin-right: 10px;"></a>
            </div>
        </div>
    </header>

    <div style="margin-top: 73px;"></div>
    
    <div id="wrapper">
        <nav class="navbar-default navbar-static-side">
            <div class="">
                <ul class="nav metismenu" id="side-menu">
                    <li>
                        <a class="link-menu" href="vizualizar.php?tpoForm=Info"><span class='nav-label'>Informações Gerais</span></a>
                    </li>                    
                    <?php 
                    for ($i=0;$i<count($metas);$i++){
                        echo "<li>";
                        echo "<a title='{$metas[$i]['metchamada']}' alt='{$metas[$i]['metchamada']}' class=\"link-menu\" href=\"vizualizar.php?metid=".$metas[$i]['metid']."&tpoForm=Quest\"><span class='nav-label' >".$metas[$i]['metchamada']."</span></a>";
                        echo "</li>";
                    }                    
                    ?>      
                </ul>
            </div>
        </nav>


        <div id="page-wrapper" class="gray-bg dashbard-1">
            <div class="fadeInRight animated">

                <div class="container">
                    <?php if (!empty($questionario->queid) && !empty($particiante->parid)) : ?>
                        <div class="col-lg-12 col-sm-12 col-xs-12 text-left">
                            <h2>Olá, <small><?php echo $particiante->parnome; ?></small>
                                <button title="Sair" id="btn-sair" class="btn btn-danger pull-right btn-sair sair" style="margin-right: -24px">
                                    <span class="fa fa-power-off"></span> Sair
                                </button>
                            </h2>
                        </div>
                        <div class="row">
                            <div class="col-lg-12 col-sm-12 col-xs-12">
                                <?php $dados = is_array($dados) ? $dados : array(); ?>                                
                                <?php 
                                if ($_REQUEST['tpoForm']=='Quest'){
                                    include_once 'vizualizarQuestionario.php';
                                }else{
                                    include_once 'vizualizarInfoParticipante.php';
                                }
                                ?>                                                                    
                            </div>
                        </div>
                    <hr>
                    <?php require "footer.php"; ?>
                </div>
                <?php else : ?>
                    <h1 class="alert alert-danger">CPF informado não condiz com o autor do questionário </h1>
                <?php endif; ?>
                <?php if ($particiante->parrepresentacao != 3) : ?>
                    <script>$(document).ready(function() { $('#representacao').trigger('change'); }) </script>
                <?php endif; ?>
            </div>
        </div>
    </div>
</body>
</html>