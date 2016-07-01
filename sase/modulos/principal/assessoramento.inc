<?php

ob_start();



// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/Assessoramento.class.inc';
include_once '../../sase/classes/SituacaoAssessoramento.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

if ($_REQUEST['acao']){
    switch($_REQUEST['acao']){
        case 'salva_arquivo':
            if ($_FILES['file']['size'] > 0){
                $Assessoramento = new Assessoramento();
                $Assessoramento->carregarPorId($_REQUEST['assid']);
                $assid       = $_REQUEST['assid'];

                $nomeArquivo = $_REQUEST['aiddsc'];
                $descricao   = explode(".", $_FILES['file']['name']);
                $campos      = array();
                $file        = new FilesSimec("assessoramento", $campos, 'sase');

                if($Assessoramento->arAtributos['assleipne'] != null){
                    $sql = "update sase.assessoramento set
                              assleipne = null
                            where assid = {$assid}";
                    $res = $db->executar($sql);
                    $file->setPulaTableEschema(true);
                    $file->setRemoveUpload($Assessoramento->arAtributos['assleipne']);
                }

                $nomeArquivo = $file->setUpload($_FILES['file']['name'], null, false);
                $assleipne   = $file->getIdArquivo();
                if($nomeArquivo) {
                    $sql = "update sase.assessoramento set
                              assleipne = {$assleipne}
                            where assid = {$assid}";
                    $res = $db->executar($sql);
                    echo <<<HTML
                    <script>
                        alert('Arquivo salvo com sucesso.');
                        window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid={$assid}';
                    </script>
HTML;
                }
            }
            break;

        case 'apaga_arquivo':
            $Assessoramento = new Assessoramento();
            $Assessoramento->carregarPorId($_REQUEST['assid']);
            if($Assessoramento->arAtributos['assleipne'] != null){
                $campos      = array();
                $file        = new FilesSimec("assessoramento", $campos, 'sase');
                $file->setPulaTableEschema(true);
                $assid = $_REQUEST['assid'];
                $sql = "update sase.assessoramento set
                              assleipne = null
                            where assid = {$assid}";
                $res = $db->executar($sql);
                $file->setRemoveUpload($_REQUEST['arqid']);
                echo <<<HTML
                    <script>
                        alert('Arquivo apagado com sucesso.');
                        window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid={$assid}';
                    </script>
HTML;
            }
            break;

        case 'download_arquivo':
            //$Assessoramento->carregarPorId($_REQUEST['assid']);
            $campos = array();
            $file   = new FilesSimec("assessoramento", $campos, 'sase');
            $arqid  = $_REQUEST['arqid'];
            if ($arqid != ''){
                $file->getDownloadArquivo($arqid);
            }
            break;

        case 'download_zip':
            ob_clean();
            $zip = new ZipArchive();
            $zname = date('d-m-Y H-i-s').".zip";
            $res = $zip->open($zname, ZipArchive::CREATE);

            $sql = "select assleipne, arq.arqdescricao from sase.assessoramento ass inner join public.arquivo arq on ass.assleipne = arq.arqid where assleipne is not null";
            $arqs = $db->carregar($sql);
            $i = 1;
            foreach ($arqs as $arq) {
                $caminho = APPRAIZ . 'arquivos/sase/' . floor($arq['assleipne'] / 1000) . '/' . $arq['assleipne'];
                if(file_exists($caminho)){
                    $zip->addFromString(basename($caminho), file_get_contents($caminho));
                    $zip->renameName($arq['assleipne'], $arq['arqdescricao']);
                }
                $i++;
            }
//            ver(filesize($zname), d);
            $zip->close();
            header('Content-Type: application/zip');
            header('Content-disposition: attachment; filename='.$zname);
            readfile($zname);
            exit(0);
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
// include APPRAIZ . 'www/sase/_funcoes.php';
// --

$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

// --------------- Decisão da Página
$pagina = "";
if( !$_GET['aba'] || $_GET['aba'] == 'lista' ) $pagina = "listaassessoramento";
if( $_GET['aba'] == 'edicao' ) $pagina = "edicaoassessoramento";
if( $_GET['aba'] == 'mapa' ) $pagina = "mapa";
// --

if( array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
	&& array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false )
	$pagina = 'mapa';

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
 $titulo = 'Assistência Técnica <br/> Município';
//$titulo = 'Assistência Técnica';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>

<div id="menu_sistema">
	<ul class="nav nav-tabs">
		<?php if( (array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
				  && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) ){ }else{ ?>

	  		<li <?=($pagina=='listaassessoramento')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramento&acao=A&aba=lista">Lista</a></li>

	  	<?php } ?>

        <?php if( (array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
                && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) || (array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)) ){ }else{ ?>
	  	
	  		<li <?=($pagina=='edicaoassessoramento')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramento&acao=A&aba=edicao">Assistência</a></li>
	  		
	  	<?php } ?>

        <?php if (!array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)){ ?>
	  	<li <?=($pagina=='mapa')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramento&acao=A&aba=mapa">Mapa</a></li>
        <?php } ?>
	</ul>
</div><?php
// --

// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/assessoramento/" . $pagina . ".inc";
// --

