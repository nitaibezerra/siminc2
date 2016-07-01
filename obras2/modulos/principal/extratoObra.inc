<?php
verificaSessao('obra');
$obrid = $_SESSION['obras2']['obrid'];

//não permitir edição para perfil CALL CENTER demanda: 311298
if(possui_perfil(array(PFLCOD_CALL_CENTER) )){
     $blockEdicao = TRUE;
}

// realiza as ações da tela de acordo com o parâmetro passado
switch( $_POST['requisicao'] ) {
    case 'visualizar': 
        include_once APPRAIZ . 'obras2/modulos/principal/visualizarExtratoDaObra.inc';
        die;
    break;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'www/obras2/_funcoes_extrato_da_obra.php';
echo '<br>';
$_SESSION['obras2']['empid'] = ($_REQUEST['empid'] ? $_REQUEST['empid'] : $_SESSION['obras2']['empid']);

if ($_GET['acao'] != 'V') {

    $arMenuBlock = bloqueiaMenuObjetoPorSituacao( $obrid );
    //Gestor MEC, Super Usuario, Supervisor Unidade, Gestor Unidade.
    
    if (!$obrid && !$empid) {
        $db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP, $url, $parametros, $arMenuBlock);
    } elseif ($obrid) {
        if ($orgid == ORGID_EDUCACAO_BASICA) {
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros, $arMenuBlock);
        } else {

            $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros, $arMenuBlock);
        }
    } else {
        $db->cria_aba(ID_ABA_CADASTRA_OBRA, $url, $parametros, $arMenuBlock);
    }

    $habilitado = true;
    $habilita = 'S';
} else {
?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $_SESSION['obras2']['obrid'] = ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
    $db->cria_aba($abacod_tela, $url, $parametros);
    $somenteLeitura = false;
    $habilitado = false;
    $habilita = 'N';
}

echo cabecalhoObra($obrid);
monta_titulo('Extrato da Obra', '');
?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script>jQuery.noConflict();</script>
<form action="" method="POST" id="formExtrato" name="formExtrato">
    <input type="hidden" value="visualizar" name="requisicao" id="requisicao"/>
    <input type="hidden" value="<?php echo $obrid; ?>" name="obrid" id="obrid"/>
    <input type="hidden" name="fotoselecionadas" id="fotoselecionadas">
    <input type="hidden" name="supids" id="supids">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtitulodireita" width="190px;">Informações da Obra</td>
            <td>
            <?php
                include_once APPRAIZ . 'includes/Agrupador.php';
                // Início dos agrupadores
                $agrupador = new Agrupador('formExtrato', '');

                // Dados padrão de destino (nulo)
                $destino = isset($agrupador2) ? $agrupador2 : array();

                // Dados padrão de origem
                $origem = array(
                    'localobra' => array(
                        'codigo' => 'localobra',
                        'descricao' => 'Local da Obra'
                    ),
                    'contatos' => array(
                        'codigo' => 'contatos',
                        'descricao' => 'Contatos'
                    ),
                    'contratacao' => array(
                        'codigo' => 'contratacao',
                        'descricao' => 'Contratação'
                    ),
                    'licitacao' => array(
                        'codigo' => 'licitacao',
                        'descricao' => 'Licitação'
                    ),
                    /* 'execucao' => array(
                      'codigo'    => 'execucao',
                      'descricao' => 'Execução Orçamentária'
                      ), */
                    'projetos' => array(
                        'codigo' => 'projetos',
                        'descricao' => 'Projetos'
                    ),
                    'etapasobra' => array(
                        'codigo' => 'etapasobra',
                        'descricao' => 'Cronograma Físico-Financeiro'
                    ),
                    'restricaoobra' => array(
                        'codigo' => 'restricaoobra',
                        'descricao' => 'Restrições e Providências'
                    ),
                );

                // exibe agrupador
                $agrupador->setOrigem('naoAgrupador', null, $origem);
                $agrupador->setDestino('agrupador', null, $destino);
                $agrupador->exibir();
            ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita">Coordenadas Geográficas</td>
            <td>
                <input type="radio" name="coordenada" id="coordenadasim" value="1" onclick="abreDados(this.value, 'coordenadas');"> Sim
                <input type="radio" name="coordenada" id="coordenadanao" value="0" onclick="abreDados(this.value, 'coordenadas');" checked="checked"> Não
            </td>
        </tr>
        <tr id="trMapa" style="display: none;">
            <td class="subtitulodireita">Imprimir Mapa</td>
            <td>
                <input type="radio" name="mapa" id="mapasim" value="1"> Sim
                <input type="radio" name="mapa" id="mapanao" value="0" checked="checked"> Não
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita">Galeria de Fotos</td>
            <td>
                <input type="radio" name="foto" id="fotosim" value="1" onclick="abreDados(this.value, 'fotos');"> Sim
                <input type="radio" name="foto" id="fotonao" value="0" onclick="abreDados(this.value, 'fotos');" checked="checked"> Não
            </td>
        </tr>
        <tr id="trNumFotos" style="display: none;">
            <td class="subtitulodireita">Nº de fotos a ser exibido</td>
            <td>
                <?php echo campo_texto('numfotos', 'N', 'N', '', 2, 2, '', '', 'left', '', 0, 'id="numfotos"'); ?>
                <input type="hidden" name="fotoseleciona" id="fotoselecionada" value=""/>
                <img src='../imagens/consultar.gif' align="absmiddle" onclick="selecionaFotos();" style="cursor: pointer;" title="Selecionar Fotos"/>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita">Vistoria</td>
            <td>
                <input type="radio" name="vistoria" id="vistoriasim" value="1" onclick="abreDados(this.value, 'vistorias');"> Sim
                <input type="radio" name="vistoria" id="vistorianao" value="0" onclick="abreDados(this.value, 'vistorias');" checked="checked"> Não
            </td>
        </tr>
        <tr id="trVistoria" style="display: none;">
            <td colspan="2">
                <?php
            	$param = array();
            	$param['not(emsid)'] = true;
            	$param['not(smiid)'] = true;
            	
                $vistoria = new Supervisao();
                $dados = $vistoria->listaDados($obrid, $param);
                $responsaveisPossiveis = $vistoria->resgataTodosRsuidsPossiveis();

                $cabecalho = array('Ação',
                                   'Ordem',
                                   'ID',
                                   'Data Acompanhamento',
                                   'Data Inclusão',
                                   'Inserido Por',
                                   'Situação da Obra',
                                   'Responsável',
                                   'Realizada Por',
                                   '% do Acompanhamento');
            
            foreach($responsaveisPossiveis as $responsavelPossivel) { 
                
                // conta numero de dados para esse responsavel
                $quantidadeParaResponsavelAtual = 0;
                foreach ($dados as $chave) {
                    if( $chave['realizacaosupervisao_rsuid'] == $responsavelPossivel['rsuid'] )
                        $quantidadeParaResponsavelAtual++;
                }
            ?>
            <table class="tabela" align="center" style="width: 98%;<?=($responsavelPossivel['rsuid']!=1)?'display:none':''?>" id="responsavel_<?=$responsavelPossivel['rsuid']?>">
                <tr>
                    <? $i = 1; ?>
                    <? if (is_array($cabecalho)) { ?>
                    <? foreach ($cabecalho as $indices => $titulos) { ?>
                        <th><?= $titulos; ?></th>
                    <? }} ?>
                </tr>
                <?php
                if (is_array($dados)) {
                    
                    $inRegistros = count( $dados );

                    if( $quantidadeParaResponsavelAtual === 0 ) {
                            echo "<tr><td colspan='10' style='text-align:center'>Nenhum registro encontrado.</td></tr>";
                    }
                    
                    foreach($dados as $chave) {
                        // Não informado
                        if($chave['realizacaosupervisao_rsuid'] != $responsavelPossivel['rsuid']) continue;

                ?>
                <tr <?= ($i%2) ? 'bgcolor=#f0f0f0' : ''; ?>>
                    <td align="center">
                        <div align="center">
                            <input type="checkbox" class="supCheck" name="exibe_<?= $chave['supid'] ?>" onclick="exibirFotos(this, '<?= $chave['supid'] ?>');" />
                        </div>
                    </td>
                    <td align="center">
                        <?= $i; ?>
                    </td>
                    <td align="center">
                        <?= $chave['supid']; ?>
                    </td>
                    <td align="center">
                        <?= $chave['dtvistoria']; ?>
                    </td>
                    <td align="center">
                        <?= $chave['dtinclusao']; ?>
                    </td>
                    <td>
                        <?= $chave['usunome']; ?>
                    </td>
                    <td>
                        <?= $chave['stadesc']; ?>
                    </td>
                    <td>
                        <?= !($chave['vistoriador']) ? $chave['vistoriador'] : 'NÃO INFORMADO'; ?>
                    </td>
                    <td>
                        <?= $chave['responsavel']; ?>
                    </td>
                    <td align="center">
                        <?php
                            $percentual = $chave["percentual"];
                            $percentual = $percentual > 100.00 ? 100.00 : $percentual;
                            echo number_format($percentual,2,',','.');
                        ?> %
                    </td>
                </tr>
                <tr style="display:none;" id="tr_<?= $chave['supid']; ?>">
                    <td colspan="10">
                        <div style="width:100%; height:100px; overflow:auto;" id="div_<?= $chave['supid']; ?>"></div>
                    </td>
                </tr>
                <? $i++; }}} ?>

            </table>
            </td>
        </tr>
        <tr bgcolor="#D0D0D0">
            <td></td>
            <td>
                <input type="button" value="Visualizar" onclick="verExtrato();" style="cursor: pointer;"/>
                <input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
            </td>
        </tr>
    </table>
</form>
  
<script type="text/javascript" lang="javascript" >
var arrSf=[]
  , arrFs=[];
    
function selecionaFotos() {
    var prefix = location.href.split("?")[0];
    window.open(
        prefix + "?modulo=principal/selecionarFotos&acao=A"
      , "selecionarFotos"
      , "width=680, height=465, status=0, menubar=0, toolbar=0, scrollbars=1, resizable=1"
    );
}

function exibirFotos (campo, supvid) {
    
    var $supids = jQuery("#supids");
    var supDiv  = '#div_'+supvid;
    var supTr   = '#tr_'+supvid;
    var hab     = "<?php echo $blockEdicao;?>";//não permitir edição para perfil CALL CENTER demanda: 311298
    var url;
    if (campo.checked === true) {
        arrSf.push(supvid);
        if (jQuery(supDiv).html().length < 10) {
            url = "?modulo=principal/supervisao/selecionarFotos&acao=A&supid=" + supvid;
            jQuery(supDiv).load(url);
        }
       jQuery(supTr).show();
       
       //não permitir edição para perfil CALL CENTER demanda: 311298
       if(hab == 1){
            setTimeout(function(){
                jQuery("#tabelaFotos").find("input[name*='fotos']").attr('disabled','disabled');
                jQuery("table[id^='responsavel']").find('input[type=button], input[type=text], textarea, select').attr('disabled','disabled');
            }, 100);
       }
       
    } else {
        arrSf.splice(arrSf.indexOf(supvid), 1);
        apagaFotosSup(supDiv);
        jQuery(supTr).hide();
    }
    
   $supids.val(arrSf.join(","));
}

function apagaFotosSup(supDiv) {
    
    var cbObj = jQuery(supDiv).find("input[type='checkbox']");
    var $fotos = jQuery("#fotoselecionadas");
    var val;
    
    for (var i in cbObj) {
        if (cbObj.hasOwnProperty(i) && jQuery(cbObj[i]).is(":checked")) {
            
            val = jQuery(cbObj[i]).attr("id");
            
            if (false !== findArray(val, arrFs)) {
                arrFs.splice(findArray(val, arrFs), 1);
                Jquery(cbObj[i]).attr("checked", false);
            }
        }
    }
    
    $fotos.val(arrFs.join(","));
}

function findArray(search, haystack) {
    
    if (jQuery.isArray(haystack)) {
        for(var i=0; i<haystack.length; i++) {
            if (haystack[i] == search) return i;
        }
    }
    return false;
}

function salvaFotosSelecionadas(campo, id) {
    
    if (campo.checked===true) {
        if (false===findArray(id, arrFs)) arrFs.push(id);
    } else {
        (false!==findArray(id, arrFs)) && arrFs.splice(findArray(id, arrFs), 1);
    }
    
    jQuery("#fotoselecionadas").val(arrFs.join(","));
}

function verExtrato() {
    
    var formulario = document.getElementById("formExtrato")
      , agrupador = document.getElementById("agrupador")
      , janela;
    
    selectAllOptions(formulario.agrupador);
    
    if (!agrupador.value) {
        alert("É necessário selecionar pelo menos uma informação sobre a obra!");
        return false;
    }
    
    if (jQuery("input[name='vistoria']:checked").val() === '1') {
        if (!jQuery("input.supCheck").is(":checked")) {
            alert("É necessário selecionar pelo menos uma supervisão da obra!");
            return false;
        }
    }
    
    formulario.target = "visualizarExtrato";
    janela = window.open("", "visualizarExtrato", "width=780, height=465, status=1, menubar=0, toolbar=0, scrollbars=1, resizable=1");
    janela.focus();
    formulario.submit();
}

function doAction(element, value) {
    var tr = document.getElementById(element)
      , IS_VALID = 1;
    
    if (parseInt(value) === IS_VALID) {
        if (document.selection) {
            tr.style.display = "block";
        } else {
            tr.style.display = "table-row";
        }
    }else{
        tr.style.display = "none";
    }
}

function abreDados(value, namespace) {
    
    var fn = {
        "coordenadas": function() {
            return doAction("trMapa", value);
        },
        "fotos": function() {
            return doAction("trNumFotos", value);
        },
        "vistorias": function () {
            return doAction("trVistoria", value);
        },
        "default": function () {
            throw new Error("Invalid arguments in 'namespace'");
        }
    };
    
    if (typeof fn[namespace] === "function") {
        fn[namespace]();
    } else {
        fn["default"]();
    }
}
</script>