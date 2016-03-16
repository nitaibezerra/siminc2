<?php
$permissao_formulario = 'S';
$linhaHabilitada      = $permissao_formulario;

if($_REQUEST['inserirFase']){
    include APPRAIZ . 'sisplan/modulos/principal/inserirFase.inc';
    die();
}


$conid = $_REQUEST['conid'];

$contratacao = new Contratacao($conid);
if($conid){
    extract($contratacao->getDados());
}else{
    $sql = "SELECT
                stpid,
                itrid,
                atiprotocolo  AS conumerocprod,
                atidatainicio AS condatainicio,
                atidatafim    AS condatatermino,
                (SELECT
                     SUM(op.opavalor)
                 FROM
                     pde.orcamentopa op
                 WHERE
                     op.atiid = '{$_REQUEST['atiid']}') AS convalorestimado
            FROM
                pde.atividade
            WHERE
                atiid = {$_REQUEST['atiid']}";
    extract($db->pegaLinha($sql));
}

    // recuperando somente a data de inicio e fim da atividade
    $sql = "SELECT
	            atidatainicio,
	            atidatafim
	        FROM
	            pde.atividade
	        WHERE
	            atiid = {$_REQUEST['atiid']}";

    extract($db->pegaLinha($sql));

$convalorestimado = formata_valor($convalorestimado);

if ($_POST['act'] == 'salvar'){

    if($_REQUEST['conid'] == ''){
        $_REQUEST['usucpf']          = $_SESSION['usucpf'];
        $_REQUEST['constatus']       = 'A';
        $_REQUEST['condatacadastro'] = 'now()';
    }

    $arCamposNulos = array();
    if($_REQUEST['condatainicio'] == ''){
        $_REQUEST['condatainicio'] = null;
        $arCamposNulos[]           = 'condatainicio';
    }else{
        $_REQUEST['condatainicio']    = formata_data_sql($_REQUEST['condatainicio']);
    }

    $_REQUEST['convalorestimado'] = str_replace(array('.', ','), array('', '.'), $_REQUEST['convalorestimado']);
    $_REQUEST['condatatermino']   = formata_data_sql($_REQUEST['condatatermino']);

    $conid = $contratacao->popularDadosObjeto()
                         ->salvar(true, true, $arCamposNulos);

     $contratacaoFase = new ContratacaoFase();
     $contratacaoFase->desabilitaByContratacao($conid);
     foreach ($_REQUEST['facid'] as $k => $facid ){
        if($facid != ''){
            $dadosContratacao              = array();
            $dadosContratacao['cofid']      = $contratacaoFase->getIdByFaseContracacao($facid, $conid);
            $dadosContratacao['conid']      = $conid;
            $dadosContratacao['facid']      = $facid;
            $dadosContratacao['cofdatafim'] = formata_data_sql($_REQUEST['cofdatafim'][$k]);
            $dadosContratacao['cofordem']   = $k;
            $dadosContratacao['cofstatus']  = 'A';

            if($dadosContratacao['cofid'] == ''){
                $dadosContratacao['usucpf']          = $_SESSION['usucpf'];
                $dadosContratacao['cofdatacadastro'] = 'now()';
            }

            $contratacaoFase->popularDadosObjeto($dadosContratacao)->salvar();
            $contratacaoFase->setDadosNull();
        }
     }

    $contratacao->commit();
    $_REQUEST['acao'] = 'I';
    $db->sucesso( 'principal/contratacoespa', "&atiid={$_REQUEST['atiid']}");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

if( $_REQUEST['conid'] && $_REQUEST['atiid']){
    $db->cria_aba($abacod_tela, $url, "&atiid={$_REQUEST['atiid']}&conid={$_REQUEST['conid']}");
}

include APPRAIZ . 'sisplan/modulos/principal/cabecalhopa.inc';

monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" rel="stylesheet"></link>
<form method="POST" name="formulario" action="">
    <input type="hidden" name="conid" value="<?= $conid; ?>"/>
    <input type=hidden   name="act"   value="0"/>
    <input type=hidden   name="atiid" value="<?= $atiid; ?>"/>
    <input type=hidden   name="atidatainicio" id="atidatainicio" value="<?= formata_data( $atidatainicio ); ?>"/>
    <input type=hidden   name="atidatafim" id="atidatafim" value="<?= formata_data( $atidatafim ); ?>"/>

    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita">
                Situação do processo de contratação:
            </td>
            <td><?php
                $sql = "SELECT stpid AS codigo, stpdsc AS descricao FROM sisplan.situacaoprocesso WHERE stpstatus = 'A' ORDER BY 2";
                $db->monta_combo("stpid",$sql,$permissao_formulario,'&nbsp','','','','','','stpid');
                echo  obrigatorio();
            ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Forma de Contratação:
            </td>
            <td>
                <select style="width: 200px;" class="CampoEstilo" name="itrid">
                    <option value=""></option>
                    <optgroup label="Execução Direta">
                        <?php
                            $sql = sprintf( "select itrid, itrdsc from sisplan.instrumento where tpiid = 2 order by itrdsc asc" );
                        ?>
                        <?php foreach( $db->carregar( $sql ) as $tipo ): ?>
                        <option value="<?= $tipo['itrid'] ?>" <? if ($itrid == $tipo['itrid'] ) echo 'selected'; ?> ><?= $tipo['itrdsc'] ?></option>
                        <?php endforeach; ?>
                    </optgroup>
                    <optgroup label="Execução Indireta">
                        <?php
                            $sql = sprintf( "select itrid, itrdsc from sisplan.instrumento where tpiid = 1 order by itrdsc asc" );
                        ?>
                        <?php foreach( $db->carregar( $sql ) as $tipo ): ?>
                        <option value="<?= $tipo['itrid'] ?>" <? if ($itrid == $tipo['itrid'] ) echo 'selected'; ?> ><?= $tipo['itrdsc'] ?></option>
                        <?php endforeach; ?>
                    </optgroup>
                </select>
                <?=obrigatorio(); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Número do Processo IPHAN:
            </td>
            <td>
                <?= campo_texto('conumerocprod','S',$permissao_formulario,'',25,20,'#####.######/####-##',''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Início:
            </td>
            <td>
                <?= campo_data2( 'condatainicio', 'N', $permissao_formulario, '', 'S', '','verificaData(this);','','verificaData(this);','','condatainicio' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data Prevista de Término:
            </td>
            <td>
                <?= campo_data2( 'condatatermino', 'S', $permissao_formulario, '', 'S', '','verificaData(this);','','verificaData(this);','','condatatermino' ); ?>
            </td>
        </tr>
        <tr id="tr_justificativa" style="display:none;">
            <td class="SubTituloDireita">
                Justificativa:
            </td>
            <td>
                <?= campo_textarea('conjustificativa', 'S', $permissao_formulario, '', 81, 5, 500,'','','Campo de Justificativa de alteração das datas de Início e Término originais do PA.' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Valor Estimado:
            </td>
            <td>
                <?= campo_texto( 'convalorestimado', 'S', $permissao_formulario, 'Valor Total', 25, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="convalorestimado" ' ) ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Objeto:
            </td>
            <td>
                <?= campo_textarea('conobjeto', 'S', $permissao_formulario, '', 81, 5, 300 ); ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" align='right' class="SubTituloDireita" style="text-align: center; font-weight: bold">
                Cronograma
            </td>
        </tr>
        <tr>
            <td colspan="2" align='right' class="SubTituloDireita" style="text-align: center; font-weight: bold">
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <?php if($permissao_formulario == 'S') :?>
                        <tr>
                            <td>
                                <a href="#" onclick="adicionarFase();">
                                    <img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Adicionar Fase" align="top">&nbsp;&nbsp;Adicionar Fase
                                </a>
                            </td>
                        </tr>
                    <?php endif; ?>
                        <tr>
                            <td>
                                <table id="tabelaItens" width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                                    <thead>
                                        <tr id="cabecalho">
                                            <td width="10%"  valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                                                <strong>Ordem</strong>
                                            </td>
                                            <td width="10%"  valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                                                <strong>Ação</strong>
                                            </td>
                                            <td width="40%"  valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                                                <strong>Item</strong>
                                            </td>
                                            <td width="40%"  valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                                                <strong>Data Prevista de Término</strong>
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody class="fase_body">
                                        <tr id="fase_0" class="fase" style="display:none;">
                                            <td align="center">
                                                <img border="0" src="/imagens/seta_filho.gif">
                                                <?php if($linhaHabilitada == 'N'): ?>
                                                    <img border="0" src="/imagens/seta_cimad.gif">
                                                    <img border="0" src="/imagens/seta_baixod.gif">
                                                <?php else: ?>
                                                    <img border="0" src="/imagens/seta_cima.gif" title="Subir"   class="subir"  onclick="subir(this);" style='cursor:pointer;'>
                                                    <img border="0" src="/imagens/seta_baixo.gif" title="Descer" class="descer" onclick="descer(this);" style='cursor:pointer;'>
                                                <?php endif; ?>
                                            </td>
                                            <td align="center">
                                                <span>
                                                    <?php if($linhaHabilitada == 'N'): ?>
                                                        <img id="excluir_0" src='/imagens/excluir_01.gif' border='0' title='Excluir'/>
                                                    <?php else: ?>
                                                        <img id="excluir_0" src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir' onclick="excluir(this);"/>
                                                    <?php endif; ?>
                                                </span>
                                            </td>
                                            <td>
                                                <input type="hidden" id="facid_0" name="facid[]" value="">
                                                <span id="facnome_0"></span>
                                            </td>
                                            <td align="center">
                                                <?php echo campo_data2( "cofdatafim[]", 'S', $linhaHabilitada, '', 'S','','','','','',"cofdatafim_0" ); ?>
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>
                            </td>
                        </tr>
                    </table>
            </td>
        </tr>

        <tr style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%"> </td>
            <td>
                <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
                <input type="button" name="voltar" value="Voltar" onclick="javascript:location.href='?modulo=principal/contratacoespa&acao=I&atiid=<?= $_REQUEST['atiid']; ?>'"/>
            </td>
        </tr>
    </table>
</form>

<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

    $(function(){

        // verificando se deve exibir o campo justificativa
        verificaData( $('#condatainicio')[0] );

        <?php
            if($conid != ''){
                $contratacaoFase = new ContratacaoFase();
                $dados = $contratacaoFase->getContratacaoFaseByContratacao($conid);
                foreach ($dados as $dado){
                    $dado['cofdatafim'] = formata_data($dado['cofdatafim']);
                    echo "adicionaLinha({$dado['facid']}, '".addslashes($dado['facnome'])."', '{$dado['cofdatafim']}');";
                }
            }
        ?>
    });

    function verificaData(elemento){
        var hiddenData1 = '';
        var hiddenData2 = '';

        var campoData1 = elemento.value;
        var campoData2 = '';

        if(elemento.id == 'condatainicio'){
        	hiddenData1 = $('#atidatainicio').val();
        	hiddenData2 = $('#atidatafim').val();
        	campoData2  = $('#condatatermino').val();
        }else{
	        hiddenData1 = $('#atidatafim').val();
        	hiddenData2 = $('#atidatainicio').val();
        	campoData2  = $('#condatainicio').val();
        }

        if( campoData1 != hiddenData1){
                $('#tr_justificativa').show();
        }else{
            if( campoData2 == hiddenData2){
            	$('#tr_justificativa').hide();
                $('#conjustificativa').val('');
            }else{
                $('#tr_justificativa').show();
            }
        }
    }


    function adicionarFase(){
    	return windowOpen( '?modulo=principal/dadosContratacao&acao=A&atiid=<?= $_REQUEST['atiid']; ?>&inserirFase=inserir','blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function adicionaLinha(id, dsc, datafim){
        var clone = $('#fase_0').clone()
                                .attr('id', 'fase_' + id)
                                .show();

        clone.html( replaceAll (clone.html() , "document.getElementById('cofdatafim_0')", "document.getElementById('cofdatafim_" + id + "')") );

        clone.find('#facid_0').attr('id', 'facid_' + id)
                                            .attr('value',id);

        clone.find('#facnome_0').attr('id', 'facnome_' + id)
                                .attr('innerHTML', dsc);

        clone.find('#cofdatafim_0').attr('id', 'cofdatafim_' + id)
                                   .attr('value', datafim);

        $('.fase:last').after(clone);
        desabilitaFlechas();
    }

    function excluir(elem){
        $(elem).parent().parent().parent().remove();
        desabilitaFlechas();
    }

    function  subir( elem ){
        var tr = $(elem).parent().parent();
        tr.insertBefore( tr.prev() );
        desabilitaFlechas();
    }

    function descer ( elem ){
        var tr = $(elem).parent().parent();
        tr.insertAfter( tr.next() );
        desabilitaFlechas();
    }

    function desabilitaFlechas(){
        var setaSubirD  = '<img border="0" src="/imagens/seta_cimad.gif" class="subirD">';
        var setaDescerD = '<img border="0" src="/imagens/seta_baixod.gif" class="descerD">';
        var setaSubir   = '<img border="0" src="/imagens/seta_cima.gif" title="Subir"   class="subir"  onclick="subir(this);" style="cursor:pointer;">';
        var setaDescer  = '<img border="0" src="/imagens/seta_baixo.gif" title="Descer" class="descer" onclick="descer(this);" style="cursor:pointer;">';

        $('.fase_body').each(function (index, elem){
            $(elem).find('tr:visible .subirD').replaceWith(setaSubir);
            $(elem).find('tr:visible .descerD').replaceWith(setaDescer);

            $(elem).find('tr:visible .subir:first').replaceWith(setaSubirD);
            $(elem).find('tr:visible .descer:last').replaceWith(setaDescerD);
        });
    }

    function removeTrById(id){
        $('#fase_'+id).remove();
        desabilitaFlechas();
    }

    function validar_cadastro(){
    	var retorno = true;
        if (!validaBranco(document.formulario.stpid, 'Situação do processo de contratação')) return;
        if (!validaBranco(document.formulario.itrid, 'Forma de Contratação')) return;
        if (!validaBranco(document.formulario.conumerocprod, 'Número do Processo IPHAN')) return;
        if (!validaBranco(document.formulario.condatatermino, 'Data Prevista de Término')) return;
        if (!validaBranco(document.formulario.convalorestimado, 'Valor Estimado')) return;

        if( $('#tr_justificativa').is(':visible') ) {
	        if (!validaBranco(document.formulario.conjustificativa, 'Justificativa')) return;
        }

        if (!validaBranco(document.formulario.conobjeto, 'Objeto')) return;


        var dtTermino = new String($('#condatatermino').val()).split('/');
        dtTermino     = new Date(dtTermino[2], new Number(dtTermino[1])-1, dtTermino[0]);

        if($('#condatainicio').val() != ''){
            var dtInicio = new String($('#condatainicio').val()).split('/');
            dtInicio     = new Date(dtInicio[2], new Number(dtInicio[1])-1, dtInicio[0]);


            if( dtInicio > dtTermino ){
                alert('A Data de Início Contratação não pode ser maior que a Data Prevista de Término da Contratação.');
                retorno = false;
                return retorno;
            }
        }

        if($('[id^=cofdatafim_]:visible:not([readonly=readonly])').length > 0 ){
            $('[id^=cofdatafim_]:visible:not([readonly=readonly])').each(function(index, elem){
                if($(elem).val() == ''){
                    alert('Todas os campos Data Fim devem ser preenchidos.');
                    retorno = false;
                    return retorno;
                }else{
                    var dtFim = new String($(elem).val()).split('/');
                    dtFim     = new Date(dtFim[2], new Number(dtFim[1])-1, dtFim[0]);
                    if( dtFim > dtTermino ){
                        alert('Nenhuma Data Prevista de Término da Fase pode ser maior que a Data Prevista de Término da Contratação.');
                        retorno = false;
                        return retorno;
                    }
                }
            });
       }else{
            alert('A Contratação deve ter pelo menos uma Fase cadastrada.');
            retorno = false;
            return retorno;
       }

        if(retorno == false){
            return retorno;
        }

        document.formulario.act.value = 'salvar';

        document.formulario.submit();
    }
</script>