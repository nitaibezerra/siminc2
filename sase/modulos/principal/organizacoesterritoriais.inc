<?php
global $db;
ob_start();

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/Territorio.class.inc';

if ($_POST['acao']){
    switch($_POST['acao']){
        case 'carrega_orgaos':
            $estuf = $_POST['estuf'];
            if($estuf != '') {
                $sql = "SELECT org.orgid, orgdsc FROM sase.orgao org INNER JOIN sase.orgaoestado oes ON oes.orgid = org.orgid WHERE oes.estuf = '{$estuf}' ORDER BY org.orgid ASC";
            } else {
                $sql = "SELECT orgid, orgdsc FROM sase.orgao ORDER BY orgid ASC";
            }
            $res = $db->carregar($sql);

            $html = "";
            $html .= <<<HTML
            <select id="orgao" name="orgao" onchange="javascript:filtrar();" class="multiselect">
HTML;
            $i = 1;
            if(is_array($res)) {
                foreach ($res as $key => $value) {
                    $attr = $i == 1 ? 'selected' : '';
                    $html .= "<option value='" . $value['orgid'] . "' $attr >" . $value['orgdsc'] . "</option>";
                    $i++;
                }
            }

            $html .= <<<HTML
            </select>
HTML;

            echo $html;
            exit;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
// include APPRAIZ . 'www/sase/_funcoes.php';
// --


// --------------- Decisão da Página
$pagina = "";
if( !$_GET['aba'] || $_GET['aba'] == 'lista' ) $pagina = "lista";
if( $_GET['aba'] == 'edicao' ) $pagina = "organizacaoterritorial";
if( $_GET['aba'] == 'mapa' ) $pagina = "mapa";
// --


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Organizações Territoriais'; 

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>

<div id="menu_sistema">
	<ul class="nav nav-tabs">
	  <li <?=($pagina=='lista')?'class="active"':''?>><a href="sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=lista">Lista</a></li>
	  <li <?=($pagina=='organizacaoterritorial')?'class="active"':''?>>
        <a href="sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao">Território</a></li>
	  <li <?=($pagina=='mapa')?'class="active"':''?>><a href="sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=mapa">Mapa</a></li>
	</ul>
</div><?php
// --

// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/organizacoesterritoriais/" . $pagina . ".inc";
// --

