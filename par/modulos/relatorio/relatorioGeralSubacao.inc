<?php

# Inclui cabecalho
include APPRAIZ . 'includes/cabecalho.inc';

# Titulo
monta_titulo('Relatório Geral de Subação', '');

?>
<form id="formulario" name="formulario" method="post" action="">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <tr>
            <td bgcolor="#c0c0c0" colspan="2"><strong>&nbsp;</strong></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Nível:</strong></td>
            <td>
                <select name="nivel">
                    <?php 
                    $arrNivel = array("empid" => "Empenho","pagid"=> "Pagamento","prpip" => "Processo", "sbaid" => "Subações", "estuf" => "UF", "muncod" => "Município");
                    foreach ($arrNivel as $chave => $valor): ?>
                        <option value="<?php echo $chave?>" <?php echo ($_REQUEST['nivel'] == $chave) ? "selected='selected'" : '' ?>><?php echo $valor?></option>
                    <?php endforeach; ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Colunas:</strong></td>
            <td>
                <?php
                    // Dados padrão de destino (nulo)
                    $arrDestino = array(
                        'uf' => array(
                            'codigo'    => 'uf',
                            'descricao' => 'UF'
                        ),
                        'entidade' => array(
                            'codigo'    => 'entidade',
                            'descricao' => 'Município'
                        ),
                        'processo' => array(
                            'codigo'    => 'processo',
                            'descricao' => 'Processo'
                        ),
                    );
					
                    // Dados padrão de origem
                    $arrOrigem = array(
                        'documento' => array(
                            'codigo'    => 'documento',
                            'descricao' => 'Termo'
                        ),					
                        'dopvalortermo' => array(
                            'codigo'    => 'dopvalortermo',
                            'descricao' => 'Valor do Termo'
                        ),					
                        'empnumero' => array(
                            'codigo'    => 'empnumero',
                            'descricao' => 'Número do Empenho'
                        ),					
                        'valorempenho' => array(
                            'codigo'    => 'valorempenho',
                            'descricao' => 'Valor do Empenho'
                        ),					
                        'pagvalorparcela' => array(
                            'codigo'    => 'pagvalorparcela',
                            'descricao' => 'Pagamento Solicitado'
                        ),					
                        'pagid' => array(
                            'codigo'    => 'pagid',
                            'descricao' => 'ID da Solicitação'
                        ),					
                        'valorpagamento' => array(
                            'codigo'    => 'valorpagamento',
                            'descricao' => 'Pagamento Efetivado'
                        ),					
                        'vpsvinculacao' => array(
                            'codigo'    => 'vpsvinculacao',
                            'descricao' => 'Vinculação'
                        ),					
                        'acaoorcamentaria' => array(
                            'codigo'    => 'acaoorcamentaria',
                            'descricao' => 'Ação Orçamentária'
                        ),					
                        'sbdptres' => array(
                            'codigo'    => 'sbdptres',
                            'descricao' => 'PTRES'
                        ),					
                        'convlrcontrato' => array(
                            'codigo'    => 'convlrcontrato',
                            'descricao' => 'Valor dos Contratos'
                        ),					
                        'ntfvlrnota' => array(
                            'codigo'    => 'ntfvlrnota',
                            'descricao' => 'Valor das Notas'
                        ),					
                        'saldobancario' => array(
                            'codigo'    => 'saldobancario',
                            'descricao' => 'Saldo Bancário'
                        ),					
                        'dopdatafimvigencia' => array(
                            'codigo'    => 'dopdatafimvigencia',
                            'descricao' => 'Vigência'
                        ),					
                        'validacao' => array(
                            'codigo'    => 'validacao',
                            'descricao' => 'Validação'
                        ),					
                        'tipo' => array(
                            'codigo'    => 'tipo',
                            'descricao' => 'Tipo Documento'
                        ),					
                        'codigosubacao' => array(
                            'codigo'    => 'codigosubacao',
                            'descricao' => 'Código da Subação'
                        ),					
                        'subacao' => array(
                            'codigo'    => 'subacao',
                            'descricao' => 'Subação'
                        ),										
                        'ano' => array(
                            'codigo'    => 'ano',
                            'descricao' => 'Ano da Subação'
                        ),
                        'valorsubacao' => array(
                            'codigo'    => 'valorsubacao',
                            'descricao' => 'Valor da Subação'
                        ),
                        'valorempenhado' => array(
                            'codigo'    => 'valorempenhado',
                            'descricao' => 'Valor Empenhado'
                        ),
                        'valorpago' => array(
                            'codigo'    => 'valorpago',
                            'descricao' => 'Valor Pago'
                        ),
                        'situacaopagamento' => array(
                            'codigo'    => 'situacaopagamento',
                            'descricao' => 'Situação Pagamento'
                        ),
                        'esfera' => array(
                            'codigo'    => 'esfera',
                            'descricao' => 'Esfera'
                        ),
                        'ssudescricao' => array(
                            'codigo'    => 'ssudescricao',
                            'descricao' => 'Status da Subação'
                        ),
                        'itensplanejados' => array(
                            'codigo'    => 'itensplanejados',
                            'descricao' => 'QTD Itens Planejados'
                        ),
                        'itensvalidados' => array(
                            'codigo'    => 'itensvalidados',
                            'descricao' => 'QTD Itens Validados'
                        ),
                        'habilita' => array(
                            'codigo'    => 'habilita',
                            'descricao' => 'Habilitação'
                        ),
                        'pendencia_obra' => array(
                            'codigo'    => 'pendencia_obra',
                            'descricao' => 'Pendências'
                        ),
                        'connumerocontrato' => array(
                            'codigo'    => 'connumerocontrato',
                            'descricao' => 'Contrato'
                        ),
                        'ntfnumeronotafiscal' => array(
                            'codigo'    => 'ntfnumeronotafiscal',
                            'descricao' => 'Nota Fiscal'
                        ),
                        'qtd_escolas_atendidas' => array(
                            'codigo'    => 'qtd_escolas_atendidas',
                            'descricao' => 'QTD Escolas Atendidas'
                        ),
                    );

                    // exibe agrupador 
                    include APPRAIZ . 'includes/Agrupador.php';
                    $agrupador3 = new Agrupador('formulario','');
                    $agrupador3->setOrigem('origem', null, $arrOrigem);
                    $agrupador3->setDestino('destino', null, $arrDestino);
                    $agrupador3->exibir();
                ?>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0" colspan="2"><strong>Filtros:</strong></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>UF:</strong></td>
            <td>
                <?php
                $arrUF = array();
                if($_REQUEST['estuf'][0]){
                    $sql = "SELECT 
                                est.estuf AS codigo,
                                est.estdescricao AS descricao
                            FROM territorios.estado est
                            WHERE est.estuf IN ('".implode("','",$_REQUEST['estuf'])."')";
                	$arrUF = $db->carregar($sql);
                }
                
                $sqlUf = "SELECT 
                                est.estuf AS codigo,
                                est.estdescricao AS descricao
                            FROM territorios.estado est";
                
                combo_popup('estuf', $sqlUf, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, $arrUF, true, false, '', true);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Município:</strong></td>
            <td>
                <?php
                $arrMunicipio = array();
                if($_REQUEST['muncod'][0]){
                    $sql = "SELECT 
                                mun.muncod AS codigo,
                                mun.estuf || ' - ' || mun.mundescricao AS descricao
                            FROM territorios.municipio mun
                            WHERE mun.muncod IN ('".implode("','",$_REQUEST['muncod'])."')";
                	$arrMunicipio = $db->carregar($sql);
                }
                
                $sqlMunicipio = "SELECT 
                                mun.muncod AS codigo,
                                mun.estuf || ' - ' || mun.mundescricao AS descricao
                            FROM territorios.municipio mun
                            ORDER BY estuf asc";
                
                combo_popup('muncod', $sqlMunicipio, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, $arrMunicipio, true, false, '', true);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Processo:</strong></td>
            <td>
                <?php
                    $prpnumeroprocesso = simec_htmlentities($_REQUEST['prpnumeroprocesso']);
                    $empnumeroprocesso = $filtro;
                    echo campo_texto('prpnumeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', '', '', '', '', '', '', $prpnumeroprocesso); 
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Termo:</strong></td>
            <td>
                <?php
                    $dopnumerodocumento = $_REQUEST['dopnumerodocumento'];
                    echo campo_texto('dopnumerodocumento', 'N', 'S', '', 20, 50, '[#]', '', '', '', '', '', '', $dopnumerodocumento);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Esfera:</strong></td>
            <td>
                <select name="esfera">
                    <?php 
                    $arrEsfera = array("" => "Todas as Esferas", "Municipal" => "Municipal", "Estadual"=> "Estadual");
                    foreach ($arrEsfera as $chave => $valor): ?>
                        <option value="<?php echo $chave?>" <?php echo ($_REQUEST['esfera'] == $chave) ? "selected='selected'" : '' ?>><?php echo $valor?></option>
                    <?php endforeach; ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Possui pendências:</strong></td>
            <td>
                <input type="radio" value='S' name="possui_pendencia" <?php echo ($_REQUEST['possui_pendencia'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="possui_pendencia" <?php echo ($_REQUEST['possui_pendencia'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='T' name="possui_pendencia" <?php echo ($_REQUEST['possui_pendencia'] == 'T' || $_REQUEST['possui_pendencia'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Plano Interno:</strong></td>
            <td>
                
                <?php
                $arrPlanoInterno = array();
                if($_REQUEST['sbdplanointerno'][0]){
                    $sql = "SELECT DISTINCT
                                sbdplanointerno as codigo,
                                sbdplanointerno as descricao
                            FROM par.subacaodetalhe
                            WHERE sbdplanointerno IS NOT NULL
                            AND sbdplanointerno IN ('".implode("','",$_REQUEST['sbdplanointerno'])."')";
                	$arrPlanoInterno = $db->carregar($sql);
                }
                
                $sqlPlanoInterno = "SELECT DISTINCT
                                        sbdplanointerno as codigo,
                                        sbdplanointerno as descricao
                                    FROM par.subacaodetalhe
                                    WHERE sbdplanointerno IS NOT NULL";
                
                combo_popup('sbdplanointerno', $sqlPlanoInterno, 'Selecione o(s) Plano(s) Interno(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, $arrPlanoInterno, true, false, '', true);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>PTRES:</strong></td>
            <td>
                <?php
                $arrPtres = array();
                if($_REQUEST['sbdptres'][0]){
                    $sql = "SELECT DISTINCT
                                sbdptres as codigo,
                                sbdptres as descricao
                            FROM par.subacaodetalhe
                            WHERE sbdptres IS NOT NULL
                            AND sbdptres IN ('".implode("','",$_REQUEST['sbdptres'])."')";
                	$arrPtres = $db->carregar($sql);
                }
                
                $sqlPlanoInterno = "SELECT DISTINCT
                                        sbdptres as codigo,
                                        sbdptres as descricao
                                    FROM par.subacaodetalhe
                                    WHERE sbdptres IS NOT NULL";
                
                combo_popup('sbdptres', $sqlPlanoInterno, 'Selecione o(s) Plano(s) Interno(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, $arrPtres, true, false, '', true);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Status da Subação:</strong></td>
            <td>
                <?php
                $arrStatusSubacao = array();
                if($_REQUEST['ssuid'][0]){
                    $sql = "SELECT DISTINCT
                                ssuid as codigo,
                                ssudescricao as descricao
                            FROM par.statussubacao
                            WHERE ssuid IN ('".implode("','", $_REQUEST['ssuid'])."')";
                	$arrStatusSubacao = $db->carregar($sql);
                }
                
                $sqlStatusSubacao = "SELECT DISTINCT
                                ssuid as codigo,
                                ssudescricao as descricao
                            FROM par.statussubacao";
                
                combo_popup('ssuid', $sqlStatusSubacao, 'Selecione o(s) Status da Subação(ões)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, $arrStatusSubacao, true, false, '', true);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Subação:</strong></td>
            <td>
                 <?php
                $arrSubacao = array();
                if($_REQUEST['sbaid'][0]){
                    $sql = "SELECT 
                                sba.sbaid AS codigo,
                                par.retornacodigosubacao(sba.sbaid) AS descricao
                            FROM par.subacao sba
                            WHERE sba.sbaid IN ('".implode("','",$_REQUEST['sbaid'])."')
                            AND sbastatus = 'A'";
                	$arrSubacao = $db->carregar($sql);
                }
                
                $sqlSubacao = "SELECT 
                                sba.sbaid AS codigo,
                                par.retornacodigosubacao(sba.sbaid) AS descricao
                            FROM par.subacao sba
                            WHERE sbastatus = 'A' LIMIT 50";
                
                combo_popup('sbaid', $sqlSubacao, 'Selecione a(s) Subação(ões)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, $arrSubacao, true, false, '', true);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><strong>Termo validado:</strong></td>
            <td>
                <input type="radio" value='S' name="termo_validado" <?php echo ($_REQUEST['termo_validado'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="termo_validado" <?php echo ($_REQUEST['termo_validado'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='T' name="termo_validado" <?php echo ($_REQUEST['termo_validado'] == 'T' || $_REQUEST['termo_validado'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0"></td>
            <td align="left" bgcolor="#c0c0c0">
                <input type="button" name="btnPesquisar" id="btnPesquisar" value="Pesquisar" />
                <input type="button" name="btnExportar" id="btnExportar" value="Exportar" />
                <input type="hidden" name="acaoBotao" value="" />
            </td>
        </tr>
    </table>
</form>

<?php
# Filtros da pesquisa
if (isset($_REQUEST['acaoBotao'])) {
    global $db;
    #Instancia de ProcessoPar
    $processoPar = new ProcessoParControle();
    $_REQUEST['arrParam'] = array_merge($arrDestino, $arrOrigem);
    $mixResultado = $processoPar->relatorioGeralSubacao($_REQUEST, TRUE);

    $arrCabecalho = $processoPar->montarCabecalho($_REQUEST['arrParam'], $_REQUEST['destino']);
    if ($_REQUEST['acaoBotao'] == 'btnExportar') {#Excel
        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_RelatGeralSubacao" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_RelatGeralSubacao" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");
        $db->monta_lista_tabulado($mixResultado, $arrCabecalho, 1000000, 5, 'N', '100%');
        exit;
    } else { #Listagem
        $db->monta_lista($mixResultado, $arrCabecalho, 50, 15, '', 'center', 'N', '', '', '', '', '');
    }
}
?>

<script type="text/javascript">
    jQuery(document).ready(function () {
        jQuery('#btnPesquisar, #btnExportar').click(function(){
            var estuf = document.getElementById('estuf');
            selectAllOptions( estuf );
            
            var muncod = document.getElementById('muncod');
            selectAllOptions( muncod );
            
            var sbaid = document.getElementById('sbaid');
            selectAllOptions( sbaid );
            
            var sbdptres = document.getElementById('sbdptres');
            selectAllOptions( sbdptres );
            
            var sbdplanointerno = document.getElementById('sbdplanointerno');
            selectAllOptions( sbdplanointerno );
            
            var origem = document.getElementById('origem');
            selectAllOptions( origem );
            
            var destino = document.getElementById('destino');
            selectAllOptions( destino );
            
            var ssuid = document.getElementById('ssuid');
            selectAllOptions( ssuid );

            jQuery('input[name=acaoBotao]').val(jQuery(this).attr('id'));
            jQuery('#formulario').submit();
        });
    });
</script>