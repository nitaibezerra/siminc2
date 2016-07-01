<?php
if (isset($_GET['obrid'])) {
    echo '<script type="text/javascript">';
    echo 'alert("Acesso não permitido!");';
    echo 'window.close();';
    echo '</script>';
    return false;
}

ini_set('memory_limit', '2046M');
set_time_limit(0);
include APPRAIZ . 'www/obras2/_funcoes_extrato_da_obra.php';

$dadosExtratoObra = '';
$obrID = $_SESSION['obras2']['obrid'];
$orgid = $_SESSION['obras2']['orgid'];
$empid = $_SESSION['obras2']['empid'];

$dadosObra = getLocalObra($obrID, $onlyData = true);

foreach ($_POST['agrupador'] as $valor) {
    
    $obrID = filter_input(INPUT_POST, 'obrid', FILTER_SANITIZE_NUMBER_INT);
    
    switch ($valor) {
        case 'localobra':
            $dadosExtratoObra .= getLocalObra($obrID);
            break;

        case 'contatos':
            $dadosExtratoObra .= getContatosObra($obrID, $orgid);
            $dadosExtratoObra .= getResponsaveis($empid);
            break;

        case 'contratacao':
            $dadosExtratoObra .= getContratacao($dadosObra, $obrID);
            break;

        case 'etapasobra':
            $dadosEtapa = getEtapasObra($obrID);
            $dadosExtratoObra .= mountTableEtapasObra($dadosEtapa);
            break;

        case 'execucao': break;

        case 'licitacao':
            $dadosExtratoObra .= getLicitacao($obrID);
            break;

        case 'projetos':
            //$dadosExtratoObra .= 
            break;

        case 'restricaoobra':
            $dadosExtratoObra .= getProjetos($obrID);
            break;
    }
}

function addOptionsExtras(array $post, $dadosObra) {
    
    if (array_key_exists('coordenada',$post) && $post['coordenada']) {
        $dadosExtratoObra .= getLatLng($dadosObra);
        
        if (array_key_exists('mapa', $post) && $post['mapa']) {
            $dadosExtratoObra .= getMapa($dadosObra);
        }
    }
    
    if (array_key_exists('foto', $post) && $post['foto']) {
        if (array_key_exists('fotoseleciona', $post) && !empty($post['fotoseleciona'])) {
            $dadosExtratoObra .= getFotos($post['fotoseleciona']);
            //$dadosExtratoObra .= getComplementoFotos($obrID);
        }
    }
    
    if (array_key_exists('vistoria', $post) && $post['vistoria']) {
        $supids = filter_input(INPUT_POST, 'supids', FILTER_SANITIZE_STRING);
        $obrid = filter_input(INPUT_POST, 'obrid', FILTER_SANITIZE_NUMBER_INT); 
        if (!empty($supids) && !empty($obrid)) {
            $dadosExtratoObra .= getVistorias($obrid, $supids);
        }
    }
    
    return $dadosExtratoObra;
}

isset($_POST) && $dadosExtratoObra .= addOptionsExtras($_POST, $dadosObra);

echo '<center>' . monta_cabecalho_relatorio(95) . '</center>';
?>
<html>
    <head>
        <title>SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação</title>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/prototype.js"></script>
        <script type="text/javascript" src="../includes/entidades.js"></script>
        <script type="text/javascript" src="/includes/estouvivo.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
        <style type="text/css">
            @media print { .notprint { display: none } }
        </style>
    </head>
    <body>
        <table class="tabela" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="subtitulocentro" colspan="2" height="25px;">Dados da Obra</td>
            </tr>
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;" width="190px;">Tipo de ensino</td>
                <td bgcolor="#f5f5f5">
                    <?php
                    if ($orgid) {
                        $sql = "SELECT orgdesc as descricao FROM obras2.orgao WHERE orgid = {$orgid}";
                        echo $db->pegaUm($sql);
                    }
                    ?>
                </td>
            </tr>	
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;">Unidade Implantadora</td>
                <td>
                <?php
                $sql = "SELECT entnome FROM entidade.entidade WHERE entid = {$dadosObra['entid']}";
                echo $db->pegaUm($sql);
                ?>
                </td>
            </tr>
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;">Esfera</td>
                <td bgcolor="#f5f5f5">
                <?php
                $arEsfera = array('M' => 'Municipal', 'E' => 'Estadual', 'F' => 'Federal');
                echo $arEsfera[$dadosObra['empesfera']];
                ?>
                </td>
            </tr>
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;">Nome da Obra</td>
                <td>
                    <?php
                    echo "(".$obrID.") " . $dadosObra['obrnome'];
                    ?>
                </td>
            </tr>
            <tr>
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;">Tipo de Obra</td>
                <td>
                    <?php
                    $tobid = (int) $dadosObra['tobid'];
                    $sql = "SELECT tobdesc FROM obras2.tipoobra WHERE tobid = $tobid";
                    echo $db->pegaUm($sql);
                    ?>
                </td>
            </tr>
            <? if (false) : ?>
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;">Subação</td>
                <td bgcolor="#f5f5f5">
                    <?php
//                    if ($prfid = $dobras->getPrfId()) {
//                        $sql = "SELECT prfdesc as descricao FROM obras.programafonte WHERE prfid = $prfid";
//                        echo $db->pegaUm($sql);
//                    } else {
//                        echo 'Não informado';
//                    }
                    ?>
                </td>
            </tr>
            <? endif; ?>
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;">Classificação da Obra</td>
                <td>
                    <?php
                    if ($dadosObras['cloid']) {
                        $sql = "SELECT clodsc AS descricao FROM obras2.classificacaoobra WHERE cloid = {$dadosObras['cloid']}";
                        echo $db->pegaUm($sql);
                    } else {
                        echo 'Não informado';
                    }
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;">Tipologia da Obra</td>
                <td bgcolor="#f5f5f5">
                    <?php
                    if ($dadosObra['tpoid']) {
                        $sql = "SELECT tpodsc AS descricao FROM obras2.tipologiaobra WHERE tpoid = {$dadosObra['tpoid']}";
                        echo $db->pegaUm($sql);
                    } else {
                        echo 'Não informado';
                    }
                    ?>
                </td>
            </tr>						
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;">Descrição / Composição da Obra</td>
                <td align="justify">
                    <?php
                    echo ($dadosObra['obrdsc']) ? $dadosObra['obrdsc'] : ($dadosObra['empcomposicao']) ? $dadosObra['empcomposicao'] : 'Não Informado';
                    ?>
                </td>
            </tr>	
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;">Observação sobre a Obra</td>
                <td bgcolor="#f5f5f5" align="justify">
                    <?php
                    echo $dadosObra['obrdsc'] ? $dadosObra['obrdsc'] : 'Não Informado';
                    ?>
                </td>
            </tr>	    		
            <tr>
                <td class="subtitulodireita" style="font-weight: bold;">Valor Previsto (R$):</td>
                <td bgcolor="#f5f5f5" align="justify">
                    <?php
                    echo $dadosObra['ocrvalorexecucao'] ? number_format($dadosObra['ocrvalorexecucao'], 2, ',', '.') : 'Não Informado';
                    ?>
                </td>
            </tr>
            
            
            
            <?php 
            
            $obraObj = new Obras($obrid);
           echo $obraObj->getLinhaTabelaPercentExecutado($obrid);?>
            
            
            
            <?= $dadosExtratoObra; ?>
        </table>
        <table class="tabela" cellSpacing="1" cellPadding="3" align="center">	
            <tr bgcolor="#D0D0D0">
                <td>
                    <input type="button" value="Imprimir" onclick="self.print();" style="cursor: pointer;"/>
                    <input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </body>
</html>