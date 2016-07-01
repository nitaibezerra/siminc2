<?php

function pegaObrasUsoCronograma ($cpmid){
    global $db;

    $cpm = new Cronograma_PadraoMi($_GET['cpmid']);    
    if(!empty($cpm->tpoid)){
        $strTpoid = " AND o.tpoid = " . $cpm->tpoid;
    }else{
        $strTpoid = "";
    }    
    
    $sql = "SELECT o.obrid, o.obrnome, '('|| o.obrid ||') ' || o.obrnome as nome FROM obras2.obras o
              JOIN workflow.documento d ON d.docid = o.docid
              JOIN entidade.endereco e ON o.endid = e.endid AND e.endstatus = 'A'
              JOIN obras2.ordemservicomi os ON os.obrid = o.obrid AND os.tomid = 1
              JOIN workflow.documento dos ON dos.docid = os.docid AND dos.esdid = 905
            WHERE d.esdid = 690 ".$strTpoid." AND o.obrstatus = 'A' AND estuf = '" . $cpm->estuf . "'";
    $obras = $db->carregar($sql);

    if($obras > 0){
        return $obras;
    }else{
        return false;
    }
}

function verificaUsoCronogramaPadrao($cpmid){
    global $db;

    $cpm = new Cronograma_PadraoMi($_GET['cpmid']);
    
    if(!empty($cpm->tpoid)){
        $strTpoid = " AND o.tpoid = " . $cpm->tpoid;
    }else{
        $strTpoid = "";
    }    
    
    $sql = "SELECT count(*) FROM obras2.obras o
              JOIN workflow.documento d ON d.docid = o.docid
              JOIN entidade.endereco e ON o.endid = e.endid AND e.endstatus = 'A'
            WHERE d.esdid = 690 ".$strTpoid." AND o.obrstatus = 'A' AND estuf = '" . $cpm->estuf . "'";

    $obras = $db->pegaUm($sql);

    if($obras > 0){
        return true;
    }else{
        return false;
    }
}

if($_REQUEST['requisicao']){
    $usoCronograma = ($_GET['cpmid']) ? verificaUsoCronogramaPadrao($_GET['cpmid']) : false;

    $icm = new Itens_Composicao_PadraoMi();
    $icm->salvarFormulario();

    $obras = ($_GET['cpmid']) ? pegaObrasUsoCronograma($_GET['cpmid']) : array();
    if($obras){
        foreach ($obras as $obra){
            $o = new Obras($obra['obrid']);
            $o->exportarCronogramaPadraoParaObra($obra['obrid']);
        }
    }

    if($_REQUEST['cpmid']){
        $_SESSION['obras2']['pagamento']['alert'] = "Formulário atualizado com sucesso!";
    }else{
        $_SESSION['obras2']['pagamento']['alert'] = "Formulário inserido com sucesso!";
    }
    header("Location: obras2.php?modulo=principal/listaItensComposicaoMI&acao=A");
    exit;

}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametro);

if($_GET['cpmid'] && !$_GET['copiar'])
    monta_titulo( "Editar Formulário MI", '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );
else if($_GET['copiar'])
    monta_titulo( "Dupicar Formulário MI", '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );
else
    monta_titulo( "Cadastrar Formulário MI", '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );

$usoCronograma = false;

if ($_GET['cpmid']) {
    $usoCronograma = verificaUsoCronogramaPadrao($_GET['cpmid']);

    $cpm = new Cronograma_PadraoMi($_GET['cpmid']);

    extract($cpm->getDados());
    
    $icm = new Itens_Composicao_PadraoMi();
    $icplist = $icm->pegaPorCpm($_GET['cpmid']);
    $icplistForaEdificacao = $icm->pegaPorCpm($_GET['cpmid'],'F');
}

$sql = "
    SELECT 
        umdid as codigo,
        umdeesc as descricao
    FROM obras2.unidademedida
    ORDER BY umdeesc ASC
";
$unidadesDeMedida = $db->carregar($sql);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<style>
    .hidden{display:none}
    .link{cursor:pointer}
</style>
<form id="formulario" name="formulario" method="post" action="" enctype="multipart/form-data" >
<?php if(!$_GET['copiar']): ?>
    <input type="hidden" name="cpmid" id="cpmid" value="<?php echo $cpmid ?>" />
<?php endif; ?>
<input type="hidden" name="icmremovidos" id="icmremovidos" value="" />
<input type="hidden" name="requisicao" id="requisicao" value="" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        <?php if($usoCronograma):?>

            <?php if(possui_perfil(array(PFLCOD_SUPER_USUARIO))): ?>
                <tr>
                    <td colspan="2" style="color:red; font-weight: bold; font-size: 13px;" class="SubTituloDireita">
                        Ao editar este cronograma, as seguintes obras serão atualizados:

                        <?php $obras = pegaObrasUsoCronograma($_GET['cpmid']); ?>
                        <ul>
                        <?php if (!empty($obras)): ?>
                            <?php foreach ($obras as $obra):?>
                            <li><?=$obra['nome']?></li>
                            <?php endforeach; ?>
                        <?php endif; ?>
                        </ul>

                    </td>
                </tr>
            <?php else: ?>

            <tr>
                    <td colspan="2" style="background: RED; color:#FFF; font-weight: bold; text-align: center; font-size: 13px;">
                            Não é possível editar o cronograma, ele está sendo usado por outras obras.
                    </td>
            </tr>

            <?php endif;?>
        <?php endif;?>
        <tr>
        <td class="SubTituloDireita" width="35%">UF</td>
        <td>
            <?php
                $uf = new Estado(); 
                if($_GET['copiar'])
                    $estuf = null;
                $db->monta_combo("estuf", $uf->listaCombo(), 'S','Selecione...','', '', '',200,'S', $estuf ); 
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Tecnologia</td>
        <td>
            <?php
                $tecnologia = new TecnologiaMi();                   
                $db->monta_combo("tmiid", $tecnologia->listaCombo(), 'S','Selecione...','', '', '',200,'S', $tmiid); 
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Tipologia</td>
        <td>
            <?php
                $tipologia = new TipologiaObra();                   
                $db->monta_combo("tpoid", $tipologia->listaCombo( array('orgid' => 3, 'tpostatus' => 'A', 'tpodsc' => 'MI') ), 'S','Selecione...','', '', '',200,'S', $tpoid); 
            ?>
        </td>
    </tr>
    
    <tr>
        <td class="SubTituloDireita">Valor</td>
        <td>
            <?php $cpmvalor = number_format($cpmvalor, 2, ',', '.') ?>
            <?php echo campo_texto( 'cpmvalor', 'S', 'S', '', 15, 30, '####.###,##', '', 'right', '', 0, ''); ?>
        </td>
    </tr>
    <tr>
        <td colspan="2">
            <div style="background:#dcdcdc;margin-top:5px;padding:4px;text-align:center;font-weight:bold;">Edificação</div>
            <table class="tabela listagem" style="width:100%; text-align: center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
                <thead>
                    <tr>
                        <td>&nbsp;</td>
                        <td><b>Item</b></td>
                        <td><b>Dia de início</b></td>
                        <td><b>Dias de duração</b></td>
                        <td><b>Valor</b></td>
                        <td><b>Percentual</b></td>
                        <td><b>Ordem</b></td>
                    </tr>
                </thead>
                <tbody id="listaitens">
                    <?php if(!$icplist): ?>
                        <tr class='estruturapadrao'>
                            <td>
                                <a href="#" class="remover" rel="" ><img src="/imagens/excluir.gif" style="cursor:pointer;" border="0" title="Excluir"></a>
                                <input name="relativoEdificacao[]" type="hidden" value="D"/>
                            </td>
                            <td>
                            <?php
                                $itens = new ItensComposicao();                 
                                $db->monta_combo("itcid[]", $itens->listaCombo(), 'S','Selecione...','', '', '',200,'S','itcid'); 
                            ?>
                            </td>
                            <td><?php echo campo_texto( 'icmdiasinicio[]', 'S', 'S', '', 9, 30, '#########', '', 'right', '', 0, ''); ?></td>
                            <td><?php echo campo_texto( 'icmdiasduracao[]', 'S', 'S', '', 9, 30, '#####', '', 'right', '', 0, '','',null); ?></td>
                            <td><?php echo campo_texto( 'icmvalor[]', 'S', 'S', '', 9, 30, '####.###,##', '', 'right', '', 0, '','',null,'calculaPercentualValor( this )'); ?></td>
                            <td><?php echo campo_texto( 'icmpercentual[]', 'S', 'N', '', 9, 30, '', '', 'right', '', 0, ''); ?></td>
                            <td>
                                <img src="../imagens/seta_cima.gif" style="cursor:pointer" onclick="javascript:mudaOrdem('cima',this)"/>&nbsp;
                                <img src="../imagens/seta_baixo.gif" style="cursor:pointer" onclick="javascript:mudaOrdem('baixo',this)"/>

                                <input name="icmquantidade[]" type="hidden" />
                                <input name="umdid[]" type="hidden" />
                                <input name="icmvalortotal[]" type="hidden" />
                            </td>
                        </tr>
                    <?php else: ?>
                        <?php foreach($icplist as $icm): ?>
                            <?php if(!$_GET['copiar']): ?>
                                <input type="hidden" name="icmid[]" id="icmid" value="<?php echo  $icm['icmid'] ?>" />
                            <?php else: ?>
                                <input type="hidden" name="icmid[]" id="icmid" value="" />
                            <?php endif; ?>
                        <tr class='estruturapadrao'>
                            <td>
                                <?php if(!$_GET['copiar']): ?>
                                    <a href="#" class="remover" rel="<?=$icm['icmid']?>" ><img src="/imagens/excluir.gif" style="cursor:pointer;" border="0" title="Excluir"></a>
                                <?php else: ?>
                                    <a href="#" class="remover" rel="" ><img src="/imagens/excluir.gif" style="cursor:pointer;" border="0" title="Excluir"></a>
                                <?php endif; ?>
                                <input name="relativoEdificacao[]" type="hidden" value="D"/>
                            </td>
                            <td>
                            <?php
                                $itens = new ItensComposicao();                 
                                $db->monta_combo("itcid[]", $itens->listaCombo(), 'S','Selecione...', '', '', '', 200, 'S', '', '', $icm['itcid']); 
                            ?>
                            </td>
                            <td><?php echo campo_texto( 'icmdiasinicio[]', 'S', 'S', '', 9, 30, '#########', '', 'right', '', 0, '', '', $icm['icmdiasinicio']); ?></td>
                            <td><?php echo campo_texto( 'icmdiasduracao[]', 'S', 'S', '', 9, 30, '#########', '', 'right', '', 0, '', '',$icm['icmdiasduracao']); ?></td>
                            <td><?php
                                $icmvalorconvertido = ($icm['icmvalor'])?number_format($icm['icmvalor'],2,',','.'):'';
                                echo campo_texto( 'icmvalor[]', 'S', 'S', '', 9, 30, '####.###,##', '', 'right', '', 0, '', '',$icmvalorconvertido, 'calculaPercentualValor( this )'); ?></td>
                            <td><?php echo campo_texto( 'icmpercentual[]', 'S', 'N', '', 9, 30, '', '', 'right', '', 0, '', '',$icm['icmpercentual'] . '%'); ?></td>
                            <td>
                                <img src="../imagens/seta_cima.gif" style="cursor:pointer" onclick="javascript:mudaOrdem('cima',this)"/>&nbsp;
                                <img src="../imagens/seta_baixo.gif" style="cursor:pointer" onclick="javascript:mudaOrdem('baixo',this)"/>

                                <input name="icmquantidade[]" type="hidden" />
                                <input name="umdid[]" type="hidden" />
                                <input name="icmvalortotal[]" type="hidden" />
                            </td>
                        </tr>
                        <?php endforeach; ?>
                    <?php endif; ?>
                </tbody>
                <tfoot>
                    <tr>
                        <td colspan="4" align="right">&nbsp;</td>
                        <td align="center">TOTAL R$</td>
                        <td align="center">TOTAL %</td>
                        <td>&nbsp;</td>
                    </tr>
                    <tr style="background-color:#ccc;">
                        <td colspan="4" align="right">&nbsp;</td>
                        <td align="center">
                            <input size="10" maxlength="30" type="text" disabled="disabled" style="text-align:right;" value="" id="_valueTotal" class="obrigatorio normal" name="_valueTotal" /><img border="0" src="../imagens/obrig.gif" title="Indica valor total.">
                        </td>
                        <td align="center">
                            <input size="10" maxlength="30" type="text" disabled="disabled" style="text-align:right;" value="" id="_valueTotalPorcentagem" class="disabled" name="_valueTotalPorcentagem" /><img border="0" src="../imagens/obrig.gif" title="Indica valor total.">
                        </td>
                        <td>&nbsp;</td>
                    </tr>
                    <tr>
                        <td colspan="9">
                            <a href='#' id='novoitem' title="Dentro da edificação"><img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"> Novo item</a>
                        </td>
                    </tr>
                </tfoot>
            </table>
            <div style="background:#dcdcdc;margin-top:5px;padding:4px;text-align:center;font-weight:bold;">Serviços Externos</div>
            <table class="tabela listagem" style="width:100%; text-align: center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
                <thead>
                    <tr>
                        <td>&nbsp;</td>
                        <td><b>Item</b></td>
                        <td><b>Dia de início</b></td>
                        <td><b>Dias de duração</b></td>
                        <td><b>Valor Unitário</b></td>
                        <td><b>Quantidade</b></td>
                        <td><b>Unidade de medida</b></td> 
                        <td><b>Valor Total</b></td>
                        <td><b>Ordem</b></td>
                    </tr>
                </thead>
                <tbody id="listaitensForaEdificacao">
                    <?php if(!$icplistForaEdificacao): ?>
                        <tr class='estruturapadraoForaEdificacao'>
                            <td>
                                <a href="#" class="remover" rel="" ><img src="/imagens/excluir.gif" style="cursor:pointer;" border="0" title="Excluir"></a>
                                <input name="relativoEdificacao[]" type="hidden" value="F"/></td>
                            <td>
                            <?php
                                $itens = new ItensComposicao();                 
                                $db->monta_combo("itcid[]", $itens->listaCombo(), 'S','Selecione...','', '', '',200,'N','itcid'); 
                            ?>
                            </td>
                            <td><?php echo campo_texto( 'icmdiasinicio[]', 'N', 'S', '', 9, 30, '#########', '', 'right', '', 0, ''); ?></td>
                            <td><?php echo campo_texto( 'icmdiasduracao[]', 'N', 'S', '', 9, 30, '#####', '', 'right', '', 0, '','',null); ?></td>
                            <td><?php echo campo_texto( 'icmvalor[]', 'N', 'S', '', 9, 30, '####.###,##', '', 'right', '', 0, '','',null,''); ?></td>
                            <td><?php echo campo_texto( 'icmquantidade[]', 'N', 'S', '', 9, 30, '#######,##', '', 'right', '', 0, '', ''); ?></td>
                            <td><?php echo $db->monta_combo("umdid[]", $unidadesDeMedida, 'S','Selecione...','', '', '',100,'N'); ?></td>
                            <td><?php echo campo_texto( 'icmvalortotal[]', 'N', 'S', '', 9, 30, '####.###,##', '', 'right', '', 0, '','',null,''); ?></td>
                            <td>
                                <img src="../imagens/seta_cima.gif" style="cursor:pointer" onclick="javascript:mudaOrdem('cima',this)"/>&nbsp;
                                <img src="../imagens/seta_baixo.gif" style="cursor:pointer" onclick="javascript:mudaOrdem('baixo',this)"/></td>
                        </tr>
                    <?php else: ?>
                        <?php foreach($icplistForaEdificacao as $icm): ?>
                            <?php if(!$_GET['copiar']): ?>
                                <input type="hidden" name="icmid[]" id="icmid" value="<?php echo  $icm['icmid'] ?>" />
                            <?php else: ?>
                                <input type="hidden" name="icmid[]" id="icmid" value="" />
                            <?php endif; ?>
                            <tr class='estruturapadraoForaEdificacao'>
                                <td>
                                    <?php if(!$_GET['copiar']): ?>
                                        <a href="#" class="remover" rel="<?=$icm['icmid']?>" ><img src="/imagens/excluir.gif" style="cursor:pointer;" border="0" title="Excluir"></a>
                                    <?php else: ?>
                                        <a href="#" class="remover" rel="" ><img src="/imagens/excluir.gif" style="cursor:pointer;" border="0" title="Excluir"></a>
                                    <?php endif; ?>
                                    <input name="relativoEdificacao[]" type="hidden" value="F"/>
                                </td>
                                <td>
                                <?php
                                    $itens = new ItensComposicao();                 
                                    $db->monta_combo("itcid[]", $itens->listaCombo(), 'S','Selecione...', '', '', '', 200, 'S', '', '', $icm['itcid']); 
                                ?>
                                </td>
                                <td><?php echo campo_texto( 'icmdiasinicio[]', 'N', 'S', '', 9, 30, '#########', '', 'right', '', 0, '', '', $icm['icmdiasinicio']); ?></td>
                                <td><?php echo campo_texto( 'icmdiasduracao[]', 'N', 'S', '', 9, 30, '#########', '', 'right', '', 0, '', '',$icm['icmdiasduracao']); ?></td>
                                <td><?php 
                                    $icmvalorconvertido = ($icm['icmvalor'])?number_format($icm['icmvalor'],2,',','.'):'';
                                    echo campo_texto( 'icmvalor[]', 'N', 'S', '', 9, 30, '####.###,##', '', 'right', '', 0, '', '',$icmvalorconvertido); ?></td>
                                <td><?php echo campo_texto( 'icmquantidade[]', 'N', 'S', '', 9, 30, '#######,##', '', 'right', '', 0, '', '',$icm['icmquantidade']); ?></td>
                                <td><?php echo $db->monta_combo("umdid[]", $unidadesDeMedida, 'S','Selecione...','', '', '',200,'N', '','', $icm['umdid'] ); ?></td>
                                <td><?php 
                                    $icmvalortotalconvertido = ($icm['icmvalortotal'])?number_format($icm['icmvalortotal'],2,',','.'):'';
                                    echo campo_texto( 'icmvalortotal[]', 'N', 'S', '', 9, 30, '####.###,##', '', 'right', '', 0, '','',$icmvalortotalconvertido,''); ?></td>
                                <td>
                                    <img src="../imagens/seta_cima.gif" style="cursor:pointer" onclick="javascript:mudaOrdem('cima',this)"/>&nbsp;
                                    <img src="../imagens/seta_baixo.gif" style="cursor:pointer" onclick="javascript:mudaOrdem('baixo',this)"/></td>
                            </tr>
                        <?php endforeach; ?>
                    <?php endif; ?>
                </tbody>
                <tfoot>
                    <tr>
                        <td colspan="4" align="right">&nbsp;</td>
                        <td align="center">TOTAL R$</td>
                        <td align="center">&nbsp;</td>
                        <td>&nbsp;</td>
                        <td>&nbsp;</td>
                        <td>&nbsp;</td>
                    </tr>
                    <tr style="background-color:#ccc;">
                        <td colspan="4" align="right">&nbsp;</td>
                        <td align="center">
                            <input size="10" maxlength="30" type="text" disabled="disabled" style="text-align:right;" value="" id="_valueTotalForaEdificacao" class="obrigatorio normal" name="_valueTotalForaEdificacao" /><img border="0" src="../imagens/obrig.gif" title="Indica valor total.">
                        </td>
                        <td align="center">&nbsp;</td>
                        <td>&nbsp;</td>
                        <td>&nbsp;</td>
                        <td>&nbsp;</td>
                    </tr>
                    <tr>
                        <td colspan="9" >
                            <a href='#' id='novoitemForaEdificacao' title="Dentro da edificação"><img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"> Novo item</a>
                        </td>
                    </tr>
                </tfoot>
            </table>
        </td>
    </tr>
    <tr>
        <td bgcolor="#c0c0c0" colspan="2" align="center">
            <?php if(!$usoCronograma || possui_perfil(array(PFLCOD_SUPER_USUARIO))):?>
            <input type="button" value="Salvar" name="btn_salvar" onclick="salvarFormulario();">
            <?php endif;?>
            <input type="button" value="Cancelar" name="btn_cancelar" onclick="javascript: location='obras2.php?modulo=principal/cadItensComposicaoMI&acao=A';">
        </td>
    </tr>
</table>

<br />


</form>
<script type="text/javascript">

$(function(){

    // dentro da edificacao
    $('#novoitem').live('click', function(e){
        e.preventDefault();
        $('#listaitens').append('<tr class="estruturapadrao">'+ $('.estruturapadrao:eq(0)').html() + '</tr>');
        $('#listaitens tr:last input, #listaitens tr:last select').val('');
        $('#listaitens tr:last .remover').attr('rel','');
        if($('.remover').length > 1){
            $('.remover img').attr('src', '/imagens/excluir.gif');
        }
    });

    // fora da edificação
    $('#novoitemForaEdificacao').live('click', function(e){
        e.preventDefault();
        $('#listaitensForaEdificacao').append('<tr class="estruturapadraoForaEdificacao">'+ $('.estruturapadraoForaEdificacao:eq(0)').html() + '</tr>');
        $('#listaitensForaEdificacao tr:last input, #listaitensForaEdificacao tr:last select').val('');
        $('#listaitensForaEdificacao tr:last [name^=relativoEdificacao]').val('F');
        $('#listaitensForaEdificacao tr:last .remover').attr('rel','');
        if($('.remover').length > 1){
            $('.remover img').attr('src', '/imagens/excluir.gif');
        }
    });
    
    $('.remover').live('click',function(e){
        e.preventDefault(); 
        
        if($('.remover').length < 2){
            alert('O cronograma deve conter ao menos um item.');
            return;
        } else if($('.remover').length < 3){
            $('.remover img').attr('src', '/imagens/excluir_01.gif');
        }
        
        icmid = $(this).attr('rel');
        if(!!icmid){
            itens = $('input#icmremovidos').val();
            if(!!itens){
                itens = itens.split(",");
                itens.push(icmid);
                $('input#icmremovidos').val(itens.join(","));
            } else {
                $('input#icmremovidos').val(icmid);
            }
        }

        classeLinha = '.' + $( $(this).parents('tr')[0] ).attr('class');
        if( $(classeLinha).length > 1 ){
            $(this).parents('tr')[0].remove();
            $('input[name=icmid\[\]][value='+icmid+']').remove();
        }else{
            $(classeLinha+' input:not([type=hidden]), '+classeLinha+' select').val('');
        }
        //calculaPercentual();
        updateValues();
    });
    
})

function salvarFormulario() {
    var erro = 0
      , camposPreenchidos = 0;

    // valida obrigatoriedade
    // trata de ter campos preenchidos, pq caso não tenha, não precisa ter essa validação, por porder se tratar de estar apagando campos.
    $.each( $('.estruturapadrao input:not([type=hidden]), .estruturapadrao select'), function(index, value){  
        if( $(value).val() != '' && $(value).val() != '%' ){
            camposPreenchidos++;
        }
    });
    
    if ( camposPreenchidos != 0 ) {
        $("[class~=obrigatorio]").each(function() {
            if(!this.value || this.value == "Selecione..."){
                erro = 1;
                alert('Favor preencher todos os campos obrigatórios!');
                this.focus();
                return false;
            }
        });
    }
    
    // valida valor total das edificacoes dentro
    var totalValorDentro = retornaValorTotal("#listaitens input[name='icmvalor[]']")
      , valorDeclaradoTotal = parseFloat(String($('[name^=cpmvalor]').val().replace(/[\.]/g,'').replace(',','.')));
    
    //totalValorDentro = totalValorDentro.toFixed(2);
    valorDeclaradoTotal = valorDeclaradoTotal.toFixed(2);
    valorDeclaradoTotal = formataParaMascaraNumerica(String(valorDeclaradoTotal).replace('.',','));
    
    /*console.log(totalValorDentro);
    console.log(valorDeclaradoTotal);*/
    
    if (totalValorDentro !== valorDeclaradoTotal) {
        
        var totalSoma = totalValorDentro
          , totalDeclarado = valorDeclaradoTotal;
        
        if (!confirm('A soma dos valores dos itens ('+totalSoma+') não está igual ao valor total do formulário ('+totalDeclarado+'). Deseja continuar?')) {
            return false;
        }
    }
    
    totalValorDentro = null;

    if (erro == 0) {
        $('#requisicao').val('1');
        $('#formulario').submit();
    }
}

/*
 * formata numero para o formato de mascara 'xxx.xxx.xxx' ou 'xxx.xxx.xxx,xx'
 */
function formataParaMascaraNumerica (numero) {
    var numeroFormatado = String(numero)
      , decimal = ''
      , decimalOccur = 0;
    
    if (numeroFormatado.indexOf(',') != -1) {
        decimal = numeroFormatado.split(',');
        numeroFormatado = decimal[0];
        decimal = decimal[1];
        decimalOccur = 1;
    }
    
    var parte = []
      , restante = numeroFormatado
      , continuidade = 1
      , reconstruido = '';
    
    while (continuidade == 1) {
        parte.push(restante.substring(restante.length-3,restante.length));
        restante = restante.substring(0,restante.length-3);

        if (restante.length <= 3) { 
            continuidade = 0; 
        }
    }
    
    for (var i = parte.length-1; i >= 0; i--) {
        if (i == parte.length-1) {
            reconstruido = reconstruido + parte[i];
        } else {
            reconstruido = reconstruido + '.' + parte[i];
        }
    }
    
    numeroFormatado = restante + '.' + reconstruido;
    if (decimalOccur == 1) {
        numeroFormatado = numeroFormatado + ',' + decimal;
    }
    
    return numeroFormatado;
}


function calculaPercentual() {
    var totalDias = 0
      , total = $("input[name=cpmvalor]").val().split(',')[0].replace(/[\.]/g,'');
    
    $("input[name=icmdiasduracao\[\]]").each(function(){
        totalDias += parseInt($(this).val());
    });
    
    // Calcula o percentual de cada um
    $("input[name=icmdiasduracao\[\]]").each(function(){
        var dias = $(this).val()
          //percent = (dias*total)/totalDias;
          , percent = (dias * 100)/totalDias
          , percent = mascaraglobal('###.##',percent.toFixed(2));
        
        $(this).parent().parent().find('[name=icmpercentual\[\]]').val(percent + '%');
    });
}

function calculaPercentualValor(elemento) {
    
    var valorElemento = $(elemento).val()
      , valorTotal = $('input[name=cpmvalor]').val()
      , total, item, percent;
    
    
    if (!valorElemento || !valorTotal) {
        return false;
    }
    
    // ajuste do formato
        valorElemento = valorElemento.replace(/[\.]/g,'');
        valorElemento = valorElemento.replace(',','.');

        valorTotal = valorTotal.replace(/[\.]/g,'');
        valorTotal = valorTotal.replace(',','.');

        valorElemento = parseFloat(valorElemento);
        valorTotal = parseFloat(valorTotal);
    // --
    
    total = valorTotal;
    item = valorElemento;
    percent = (item * 100)/total;
    percent = percent.toFixed(2);
    
    $(elemento).parent().parent().find('[name^=icmpercentual]').val(percent + '%');
    updateValues();
}

$("input[name='icmvalor[]']").focus(function(){
    
    var $cpmvalor = $("input[name='cpmvalor']")
      , DEFAULT_VALUE = '0,00';
      
    if ($cpmvalor.val() === DEFAULT_VALUE) {
        alert("Antes de definir o valor de um item, primeiro preencha o valor da MI.");
        $cpmvalor.focus();
    }
    
    return false;
});

function mudaOrdem(ordem, elemento) {
    var tr = $(elemento).parent().parent();
    
    if (ordem == 'baixo') {
        tr.next().after( tr );
        tr.before( tr.next() );
    }
    
    if (ordem == 'cima') {
        tr.prev().before( tr );
        tr.after( tr.prev() );
    }
}

function somaPercentual(InputSelector) {
    var total=0, cleanUp, strValue;
    
    $(InputSelector).each(function(i, e){
        cleanUp = $(e).val().replace(".","").replace("%","");
        total += parseFloat(cleanUp);
    });
    
    strValue = total.toString().substring(0, (total.toString().length - 2)) 
             + "." + total.toString().substring((total.toString().length - 2));
    
    return strValue;
}

function retornaValorTotal(selectInput) {
    var total = 0, cleanUp;
    
    $(selectInput).each(function(i, e){
//        cleanUp = $(this).val().replace(",","").replace(".","");
        cleanUp = $(this).val().replace(".","").replace(",",".");
        total += parseFloat(cleanUp);
    });
    
    var strValue = total.toString().substring(0, (total.toString().length - 2));
    return number_format(strValue, 2, ",", ".");
}

function number_format(number, decimals, dec_point, thousands_sep) {
    number = (number + '').replace(/[^0-9+\-Ee.]/g, '');
    var n = !isFinite(+number) ? 0 : +number,
        prec = !isFinite(+decimals) ? 0 : Math.abs(decimals),
        sep = (typeof thousands_sep === 'undefined') ? ',' : thousands_sep,
        dec = (typeof dec_point === 'undefined') ? '.' : dec_point,
        s = '',
        toFixedFix = function(n, prec) {
            var k = Math.pow(10, prec);
            return '' + Math.round(n * k) / k;
        };
    // Fix for IE parseFloat(0.55).toFixed(0) = 0;
    s = (prec ? toFixedFix(n, prec) : '' + Math.round(n)).split('.');
    if (s[0].length > 3) {
        s[0] = s[0].replace(/\B(?=(?:\d{3})+(?!\d))/g, sep);
    }
    if ((s[1] || '').length < prec) {
        s[1] = s[1] || '';
        s[1] += new Array(prec - s[1].length + 1).join('0');
    }
    return s.join(dec);
}

function updateValues() {
    $("#_valueTotal").val(retornaValorTotal("#listaitens input[name='icmvalor[]']"));
    $("#_valueTotalForaEdificacao").val(retornaValorTotal("#listaitensForaEdificacao input[name='icmvalor[]']"));
    
    if ($("#_valueTotal").val() > "0,00") {
        $("#_valueTotalPorcentagem").val(somaPercentual("#listaitens input[name='icmpercentual[]']"));
    }
}

$(function(){
    updateValues();
});
</script>