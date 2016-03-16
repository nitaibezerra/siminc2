<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/AssessoramentoEstado.class.inc';
include_once '../../sase/classes/SituacaoAssessoramento.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

if ($_REQUEST['acao']){
    switch($_REQUEST['acao']){
        case 'salva_arquivo':
            if ($_FILES['file']['size'] > 0){
                $AssessoramentoEstado = new AssessoramentoEstado();
                $AssessoramentoEstado->carregarPorId($_REQUEST['aseid']);
                $aseid       = $_REQUEST['aseid'];

                $nomeArquivo = $_REQUEST['aiddsc'];
                $descricao   = explode(".", $_FILES['file']['name']);
                $campos      = array();
                $file        = new FilesSimec("assessoramentoEstado", $campos, 'sase');

                if($AssessoramentoEstado->arAtributos['aseleipne'] != null){
                    $sql = "update sase.assessoramentoEstado set
                              aseleipne = null
                            where aseid = {$aseid}";
                    $res = $db->executar($sql);
                    $file->setPulaTableEschema(true);
                    $file->setRemoveUpload($AssessoramentoEstado->arAtributos['aseleipne']);
                }

                $nomeArquivo = $file->setUpload($_FILES['file']['name'], null, false);
                $aseleipne   = $file->getIdArquivo();
                if($nomeArquivo) {
                    $sql = "update sase.assessoramentoEstado set
                              aseleipne = {$aseleipne}
                            where aseid = {$aseid}";
                    $res = $db->executar($sql);
                    echo <<<HTML
                    <script>
                        alert('Arquivo salvo com sucesso.');
                        window.location.href = 'sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao&aseid={$aseid}';
                    </script>
HTML;
                }
            }
            break;

        case 'apaga_arquivo':
            $Assessoramento = new AssessoramentoEstado();
            $Assessoramento->carregarPorId($_REQUEST['aseid']);
            if($Assessoramento->arAtributos['aseleipne'] != null){
                $campos      = array();
                $file        = new FilesSimec("assessoramentoEstado", $campos, 'sase');
                $file->setPulaTableEschema(true);
                $aseid = $_REQUEST['aseid'];
                $sql = "update sase.assessoramentoEstado set
                              aseleipne = null
                            where aseid = {$aseid}";
                $res = $db->executar($sql);
                $file->setRemoveUpload($_REQUEST['arqid']);
                echo <<<HTML
                    <script>
                        alert('Arquivo apagado com sucesso.');
                        window.location.href = 'sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao&aseid={$aseid}';
                    </script>
HTML;
            }
            break;

        case 'download_arquivo':
            //$Assessoramento->carregarPorId($_REQUEST['assid']);
            $campos = array();
            $file   = new FilesSimec("assessoramento", $campos, 'sase');
            $arqid  = $_REQUEST['arqid'];
            if($arqid != '') {
                $file->getDownloadArquivo($arqid);
            }
            break;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
// include APPRAIZ . 'www/sase/_funcoes.php';
// --


// --------------- Decisão da Página
$pagina = "";
if( !$_GET['aba'] || $_GET['aba'] == 'lista' ) $pagina = "listaassessoramentoEstado";
if( $_GET['aba'] == 'edicao' ) $pagina = "edicaoassessoramentoEstado";
if( $_GET['aba'] == 'mapa' ) $pagina = "mapaEstado";
// --

$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

if( in_array(PFLCOD_SASE_CONSULTA, $perfis) ){
	$pagina = 'mapaEstado';
}

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
// $titulo = 'Assessoramento Municipal.';
$titulo = 'Assistência Técnico <br/> Estado';
//$titulo = 'Assessoria Estado';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<script>
jQuery("#loading").hide();
</script>
<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>

<div id="menu_sistema">
	<ul class="nav nav-tabs">
	  <?php if( !in_array(PFLCOD_SASE_CONSULTA, $perfis) ){ ?>
		  <li <?=($pagina=='listaassessoramentoEstado')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=lista">Lista</a></li>
		  <!-- <li <?=($pagina=='edicaoassessoramento')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramento&acao=A&aba=edicao">Assessoramento</a></li> -->
          <?php if(!in_array(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)){ ?>
		  <li <?=($pagina=='edicaoassessoramentoEstado')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao">Assistência</a></li>
          <?php } ?>
	  <?}?>
    <?php if(!in_array(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)){ ?>
	  <li <?=($pagina=='mapaEstado')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=mapa">Mapa</a></li>
    <?php } ?>
	</ul>
</div><?php
// --

// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/assessoramento/" . $pagina . ".inc";
// --

