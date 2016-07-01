<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 10:10
 */
ob_start();

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/PlanodecarreiraEstado.class.inc';
//include_once '../../sase/classes/SituacaoAssessoramento.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

include APPRAIZ . 'includes/cabecalho.inc';

$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

$pagina = "";
if( !$_GET['aba'] || $_GET['aba'] == 'lista' ) $pagina = "lista";
if( $_GET['aba'] == 'edicao' ) $pagina = "formulario";
if( $_GET['aba'] == 'mapa' ) $pagina = "mapa";

if( array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
    && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false )
    $pagina = 'mapa';

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'Plano de Carreira de Profissionais de Ensino <br/> Estado';

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

            <li <?=($pagina=='lista')?'class="active"':''?>><a href="sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=lista">Lista</a></li>

        <?php } ?>

        <?php if( (array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
                && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) || (array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)) ){ }else{ ?>

            <li <?=($pagina=='formulario')?'class="active"':''?>><a href="sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=edicao">Plano de Carreira</a></li>

        <?php } ?>

        <?php if (!array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)){ ?>
            <li <?=($pagina=='mapa')?'class="active"':''?>><a href="sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=mapa">Mapa</a></li>
        <?php } ?>
    </ul>
    </div><?php
// --

// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/planodecarreira/estado/" . $pagina . ".inc";
// --

