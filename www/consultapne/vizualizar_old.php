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
include_once "classes/Avaliacao.class.inc";
include_once "classes/Comentario.class.inc";

$item = new Item();
$avaliacao = new Avaliacao();
$comentario = new Comentario();
$questionario = new Questionario($_SESSION['queid_pne']);
$particiante = new Participante($_SESSION['parid_pne']);
$capitulos = $item->carregarItens();
$respostas = $avaliacao->carregarRespostas();

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
<html lang="pt-BR">
    <?php require "head.php"; ?>
    <body>
        <header>
            <div class="row">
                <div class="col-lg-12 col-sm-12 col-xs-12">
                    <img src="imagens/logo-simec.png" class="res" width="150">
                    <a class="brasil pull-right" href="http://www.brasil.gov.br/"><img alt="Brasil - Governo Federal" src="http://portal.mec.gov.br/templates/mec2014/images/brasil.png" style="margin-right: 10px;"></a>
                </div>
            </div>
        </header>
        <div class="container">
            <?php if (!(empty($questionario->queid) && empty($particiante->parid))) : ?>
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
                        <?php foreach ($capitulos as $capitulo) : ?>
                            <?php $sessoes = $item->carregarItens($capitulo['iteid']); ?>
                            <div class="well-sm">
                                <fieldset>
                                    <legend>
                                        <?php echo $capitulo['itedsc']; ?>
                                    </legend>
                                    <?php $sessao = current($sessoes);
                                    if('S' != $sessao['itetipo']){
                                        $sessoes = array(array('iteid'=>$capitulo['iteid'], 'itedsc' => 'Artigos'));
                                    }
                                    ?>
                                    <?php foreach ($sessoes as $sessao) : ?>
                                        <?php $artigos = $item->carregarItens($sessao['iteid']); ?>
                                        <h3>
                                            <?php echo $sessao['itedsc']; ?>
                                        </h3>
                                        <div class="panel-group" id="accordion">
                                            <?php foreach ($artigos as $artigo) : ?>
                                                <div class="panel panel-default panel-link">
                                                    <div class="panel-heading">
                                                        <h4 class="panel-title">
                                                            <a data-toggle="collapse" class="accordion" data-parent="#accordion" href="#collapse-<?php echo $artigo['iteid'] ?>">
                                                                <i class="fa fa-angle-up pull-left angle"></i>
                                                                <div style="margin-left: 24px;" class="text-justify"><?php echo $artigo['itedsc']; ?></div>
                                                            </a>
                                                        </h4>
                                                    </div>
                                                </div>
                                                <?php $incisos = $item->carregarItens($artigo['iteid'])?>
                                                <div id="collapse-<?php echo $artigo['iteid'] ?>" class="panel panel-default panel-content panel-collapse collapse <?php echo count($incisos) > 0 ? ' in' : null; ?>">
                                                    <div class="panel-body incisos">
                                                        <?php if (count($incisos)) : ?>
                                                            <?php foreach ($incisos as $inciso) : ?>
                                                                <p class="text-justify"><?php echo $inciso['itedsc']; ?></p>
                                                            <?php endforeach; ?>
                                                        <?php else: ?>
                                                            <div class="alert alert-warning">Nenhum inciso ou parágrafo vinculado a este artigo.</div>
                                                        <?php endif; ?>
                                                    </div>
                                                </div>
                                                <?php $index = array_search($artigo['iteid'], array_column($respostas, 'iteid')); ?>
                                                <?php $avaliacao = $index !== false ? $respostas[$index] : array(); ?>
                                                <?php if ($avaliacao) : ?>
                                                    <div class="avaliacao">
                                                        <?php if ($avaliacao['avaresposta']) : ?>
                                                            <div class="btn-group " data-toggle="buttons">
                                                                <label class="btn btn-default disabled <?php echo $avaliacao['avaresposta'] == 5 ? Avaliacao::$niveis[$avaliacao['avaresposta']] : null; ?>">
                                                                    <input disabled="disabled" type="radio"> Concordo totalmente
                                                                </label>
                                                                <label class="btn btn-default disabled <?php echo $avaliacao['avaresposta'] == 4 ? Avaliacao::$niveis[$avaliacao['avaresposta']] : null; ?>">
                                                                    <input disabled="disabled" type="radio"> Concordo parcialmente
                                                                </label>
                                                                <label class="btn btn-default disabled <?php echo $avaliacao['avaresposta'] == 3 ? Avaliacao::$niveis[$avaliacao['avaresposta']] : null; ?>">
                                                                    <input disabled="disabled"  type="radio"> Não concordo e nem discordo
                                                                </label>
                                                                <label class="btn btn-default disabled <?php echo $avaliacao['avaresposta'] == 2 ? Avaliacao::$niveis[$avaliacao['avaresposta']] : null; ?>">
                                                                    <input disabled="disabled" type="radio"> Discordo parcialmente
                                                                </label>
                                                                <label class="btn btn-default disabled <?php echo $avaliacao['avaresposta'] == 1 ? Avaliacao::$niveis[$avaliacao['avaresposta']] : null; ?>">
                                                                    <input disabled="disabled" type="radio"> Discordo totalmente
                                                                </label>
                                                            </div>
                                                        <?php endif; ?>
                                                        <?php if ($avaliacao['comdsc']) : ?>
                                                            <textarea disabled="disabled" class="comment" placeholder="Informe um comentário a respeito de sua avaliação para este item" rows="4"><?php echo $avaliacao['comdsc']; ?></textarea>
                                                        <?php endif; ?>
                                                    </div>
                                                <?php endif; ?>
                                            <?php endforeach; ?>
                                        </div>
                                    <?php endforeach; ?>
                                </fieldset>
                            </div>
                        <?php endforeach; ?>
                    </div>
                </div>
                <hr>
                <?php require "footer.php"; ?>
            </div>
        <?php else : ?>
            <h1 class="alert alert-danger">CPF informado não condiz com o autor do questionário </h1>
        <?php endif; ?>
    </body>
</html>