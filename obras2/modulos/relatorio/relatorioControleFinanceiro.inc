<?php

ini_set("memory_limit", "50000M");

if ($_REQUEST['requisicaoAjax']) {
    header('content-type: text/html; charset=ISO-8859-1');
    $_REQUEST['requisicaoAjax']();
    exit;
}

if (isset($_REQUEST['form']) == true) {
    montaRelatorioControleFinanceiro();
    exit;
}



include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo('Relatório de Controle Financeiro do Termo de Compromisso/Convênio', '&nbsp;');

function filtrarMunicipio() {
    #Municipio
    $munSql = " SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio ";

    $estadosFiltrar = trim($_REQUEST['estados'], ",");

    if ($estadosFiltrar) {
        $munSql.=" where estuf in (" . str_replace("\\", "", $estadosFiltrar) . ")";
    }

    mostrarComboPopup('Município', 'slMunicipio', $munSql, $stSqlCarregados, 'Selecione os municípios', null, '', true);
    die;
}
?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js">
</script>
<script type="text/javascript">

    function getEstados()
    {
        var estados = '';

        var elemento = document.getElementsByName('slEstado[]')[0];

        for (var i = 0; i < elemento.options.length; i++) {
            if (elemento.options[i].value != '')
            {
                estados += "'" + elemento.options[i].value + "',";
            }
        }

        return estados;
    }


    function ajaxEstado()
    {
        jQuery.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=filtrarMunicipio&estados=" + getEstados(),
            success: function(retorno) {
                jQuery('#idMunicipio').html(retorno);
            }});
    }


    function onOffCampo(campo)
    {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }

    /**
     * Alterar visibilidade de um bloco.	 
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco(bloco)
    {
        var div_on = document.getElementById(bloco + '_div_filtros_on');
        var div_off = document.getElementById(bloco + '_div_filtros_off');
        var img = document.getElementById(bloco + '_img');
        var input = document.getElementById(bloco + '_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }

    function gerarRelatorio(tipo) {

        var formulario = document.formulario;
        var tipo_relatorio = tipo;

        prepara_formulario();

        if (tipo_relatorio == 'visual') {

            document.getElementById('tipo_relatorio').value = 'visual';
            var janela = window.open('', 'relatorio', 'width=1150,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            formulario.target = 'relatorio';
            
            //Seta-se no atributo "notclose" o obj instanciado "popup1", a fim de que não se feche a popup
            janela.opener.notclose = janela;
        }
        else {

            document.getElementById('tipo_relatorio').value = 'xls';
            var janela = window.open('', 'relatorio', 'width=1150,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            formulario.target = 'relatorio';
        }
        formulario.submit();
    }
</script>
<form name="formulario" id="formulario" action="" method="post">

    <input type="hidden" name="form" value="1" /> 
    <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="" /> 

    <table id="tabela_filtros2" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="subtituloesquerda" colspan="2"><strong>Filtros</strong>
            </td>
        </tr>
        <tr> 
            <td style="width: 20%;" class="SubTituloDireita">ID da Obra:</td>            
            <td>
                    <?php echo campo_texto( 'obrid', 'N', 'S', '', 11 , 11, '##########', '', 'left', '', 0, 'id="obrid"'); ?>
            </td>
        </tr>
        <tr>
            <td style="width: 20%;" class="SubTituloDireita">Nº processo:</td>
            <td>
                    <?php echo campo_texto( 'nprocesso', 'N', 'S', '', 25, 25, "#####.######/####-##", '', 'left', '', 0, 'id="nprocesso"'); ?>
            </td>
        </tr>
        <tr>
            <td style="width: 20%;" class="SubTituloDireita">Esfera:</td>
            <td>
                <input type="radio" value="e" name="esfera" id="esfera_estadual"> Estadual <br/>
                <input type="radio" value="m" name="esfera" id="esfera_municipal"> Municipal
            </td>
        </tr>
        <tr>
            <!--<td colspan="2">-->
                <?php
                    #UF
                    $ufSql = " SELECT 	estuf as codigo, estdescricao as descricao
                               FROM territorios.estado est
                               ORDER BY estdescricao
                             ";
                    $stSqlCarregados = "";
                    mostrarComboPopup('Estado', 'slEstado', $ufSql, '', 'Selecione o(s) Estado(s)', null, 'ajaxEstado', false);
                ?>
            <!--</td>-->
        </tr>
        <tr id='idMunicipio'>
            <?php
                #Municipio
                $munSql = " SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio ";

                mostrarComboPopup('Município', 'slMunicipio', $munSql, $stSqlCarregados, 'Selecione os municípios', null, '', true);
            ?>
        </tr>
        <tr>
            <!--<td class="subtitulodireita">&nbsp;</td>-->
            <td align="center" colspan="2">
                <input type="button" name="Gerar Relatório"
                                     value="Gerar Relatório"
                                     onclick="javascript:gerarRelatorio('visual');" /> 
                <input type="button" name="Gerar Relatório" value="Visualizar XLS"
                                     onclick="javascript:gerarRelatorio('xls');" />
            </td>
        </tr>
    </table>
</form>

<?php


function montaRelatorioControleFinanceiro(){
    $dados = getDadosRelatorioControleFinanceiro();
    if($_REQUEST['tipo_relatorio'] == 'xls'){
        global $db;
        foreach ($dados as $key => $value) {
            unset($dados[$key]['pronumeroprocesso']);
        }
        $cabecalho = array('Número processo', 'Termo de Compromisso', 'ID da obra', "Nome da Obra", "Tipologia", "UF", "Município", "Esfera", "Situação", 'Valor Contratado Obra', 'Valor Contratado', 'Valor Pactuado', 'Valor da Parcela',
                           'Saldo', 'Referência do Saldo', 'Valor Utilizado', 'Utilizado do Contrato', '% Executado', 'Valor Executado', 'Resultado');
        $db->sql_to_xml_excel($dados,'relatorio_controle_financeiro',$cabecalho);
    }
    
    $html  = montaHTMLRelatorioControleFinanceiro($dados);
    echo $html;
    die();
}

function montaHTMLRelatorioControleFinanceiro(array $dados){
    
//    ver($dados);
    
    $html          = '';
    $html_titulos  = '';
    $html_dados    = '';
    $html_fim      = '';
    $html_blank_ln = '<tr><td colspan="13">&nbsp;</td></tr>';
    
    
    $html         = ' <!doctype>
                        <html>
                            <head>
                                <title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
                                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
                                <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
                                <script type="text/javascript">
                                function carregaObraJanelaPai(obrid){
                                    window.parent.opener.location.href = \'obras2.php?modulo=principal/cadObra&acao=A&obrid=\'+obrid;
                                }
                                function pesquisaPorProcesso(nprocesso){
                                    window.location.href = \'obras2.php?modulo=relatorio/relatorioControleFinanceiro&acao=A&form=true&nprocesso=\'+nprocesso;
                                }
                                </script>
                                
                            </head>
                            <body>
                                <center>
                                    <!--  Cabeçalho Brasão -->
                                    '.monta_cabecalho_relatorio('95').' 
                                </center>
                                <br />
                                <table cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">
                             ';
    
    $html_titulos = '  
                        <tr>
                            <th width="130px">Nº PROCESSO</th>
                            <th>Nº TERMO DE COMPROMISSO/ CONVÊNIO</th>
                            <th>ID DA OBRA</th>
                            <th>NOME DA OBRA</th>
                            <th>TIPOLOGIA</th>
                            <th>UF</th>
                            <th>MUNICÍPIO</th>
                            <th>ESFERA</th>
                            <th>SITUAÇÃO</th>
                            <th>VALOR CONTRATADO OBRA</th>
                            <th>VALOR CONTRATADO</th>
                            <th>VALOR PACTUADO</th>
                            <th>VALOR REPASSADO FNDE</th>
                            <th>SALDO EM CONTA-CORRENTE</th>
                            <th>MÊS/ANO REFERÊNCIA DO SALDO</th>
                            <th>VALOR UTILIZADO</th>
                            <th>% UTILIZADO DO CONTRATADO</th>
                            <th>% EXECUTADO</th>
                            <th>VALOR EXECUTADO</th>
                            <th>RESULTADO</th>
                        </tr>
                    ';
    
    $html_dados        = '';
    $html_dados_totais = '';
    $num_processo      = '';
    
    $total_vl_contrat   = 0;
    $total_vl_pactuado  = 0;
    $total_vl_repassado = 0;
    $total_vl_saldo     = 0; //Não soma
    $total_vl_utilizado = 0; 
    $total_per_util_con = 0; 
    $total_vl_executado = 0; 
    $total_vl_resultado = 0; 
    $total_de_obras     = 0;
    
    $line_counter    = 0;
    $process_counter = 0;
    foreach ($dados as $key => $value) {
        
        //ver($value);
        
        $bg_line = ($line_counter % 2 == 0) ? ' bgcolor="#FFFFFF" ' : '';
        
        if($line_counter == 0){
            $num_processo = $value['pronumeroprocesso'];
            $process_counter++;
        }  
        elseif ($num_processo != $value['pronumeroprocesso']) {
            $process_counter++;
            $num_processo = $value['pronumeroprocesso'];
            
            $total_vl_utilizado = $total_vl_repassado - $total_vl_saldo;
            $total_per_util_con = (empty($total_vl_contrat)) ? 0 : ($total_vl_utilizado/$total_vl_contrat)*100;
            $total_vl_resultado = $total_vl_utilizado - $total_vl_executado;
            
            $plural = ($total_de_obras > 1) ? 's' : '';
            
            $html_dados_totais .= '<tr bgcolor="#CCCCCC" >';
            
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>TOTAL:</th>';
            $html_dados_totais .= '<th> '.number_format($total_de_obras,'0',',','.').' obra'.$plural.' </th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_contratObr,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_contrat,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_pactuado,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_repassado,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_saldo,'2',',','.').' </th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_utilizado,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_per_util_con,'2',',','.').' </th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_executado,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_resultado,'2',',','.').' </th>';
            
            $html_dados_totais .= '</tr>';
            
            $html_dados .= $html_dados_totais;

            $total_vl_contratObr   = 0;
            $total_vl_contrat   = 0;
            $total_vl_pactuado  = 0;
            $total_vl_repassado = 0;
            $total_vl_saldo     = 0; //Não soma
            $total_vl_utilizado = 0; 
            $total_per_util_con = 0; 
            $total_vl_executado = 0; 
            $total_vl_resultado = 0; 
            $total_de_obras     = 0;
            $html_dados_totais  = '';
            
            if($process_counter % 15 == 0){ $html_dados .= $html_blank_ln.$html_titulos; }
        }
        
        $nprocesso = str_replace('.','',str_replace('-','',str_replace('/','',$value['Número processo'])));
        
        $html_dados   .= '<tr '.$bg_line.'>';
        $html_dados   .= '<td><span onclick="pesquisaPorProcesso(\''.$nprocesso.'\')" style="cursor:pointer;">'.$value['Número processo'].'</span></td>';
        $html_dados   .= '<td>'.$value['Termo de Compromisso'].'</td>';
        $html_dados   .= '<td><span onclick="carregaObraJanelaPai('.$value['ID da obra'].')" style="cursor:pointer;">'.$value['ID da obra'].'</span></td>';
        $html_dados   .= '<td>'.$value['Nome Obra'].'</td>';
        $html_dados   .= '<td>'.$value['Tipologia'].'</td>';
        $html_dados   .= '<td>'.$value['UF'].'</td>';
        $html_dados   .= '<td>'.$value['Município'].'</td>';
        $html_dados   .= '<td>'.$value['Esfera'].'</td>';
        $html_dados   .= '<td>'.$value['situacao'].'</td>';
        $html_dados   .= '<td>'.number_format($value['Valor Contratado Obra'],'2',',','.').'</td>';
        $html_dados   .= '<td>'.number_format($value['Valor Contratado'],'2',',','.').'</td>';
        $html_dados   .= '<td>'.number_format($value['Valor Pactuado'],'2',',','.').'</td>';
        $html_dados   .= '<td>'.number_format($value['Valor da Parcela'],'2',',','.').'</td>';
        $html_dados   .= '<td>'.number_format($value['Saldo'],'2',',','.').'</td>';
        $html_dados   .= '<td>'.$value['Referência do Saldo'].'</td>';
        $html_dados   .= '<td>'.$value['Valor Utilizado'].'</td>';
        $html_dados   .= '<td>'.$value['% Utilizado do Contrato'].'</td>';
        $html_dados   .= '<td>'.number_format($value['% Executado'],'2',',','.').'</td>';
        $html_dados   .= '<td>'.number_format($value['Valor Executado'],'2',',','.').'</td>';
        $html_dados   .= '<td>'.$value['Resultado'].'</td>';
        
        $html_dados   .= '</tr>';
        
        $total_vl_contratObr   += $value['Valor Contratado Obra'];
        $total_vl_contrat   += $value['Valor Contratado'];
        $total_vl_pactuado  += $value['Valor Pactuado'];
        $total_vl_repassado += $value['Valor da Parcela'];
        $total_vl_saldo      = $value['Saldo']; //Não soma
        $total_vl_executado += $value['Valor Executado']; 
        $total_de_obras++;
        
        //Para os casos que exibe apenas 1 processo
        if((count($dados)-1 == $key) && ($num_processo == $value['pronumeroprocesso']) ){
            
            $total_vl_utilizado = $total_vl_repassado - $total_vl_saldo;
            $total_per_util_con = (empty($total_vl_contrat)) ? 0 : ($total_vl_utilizado/$total_vl_contrat)*100;
            $total_vl_resultado = $total_vl_utilizado - $total_vl_executado;
            
            $plural = ($total_de_obras > 1) ? 's' : '';
            
            $html_dados_totais .= '<tr bgcolor="#CCCCCC" >';
            
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>TOTAL:</th>';
            $html_dados_totais .= '<th> '.number_format($total_de_obras,'0',',','.').' obra'.$plural.' </th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_contratObr,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_contrat,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_pactuado,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_repassado,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_saldo,'2',',','.').' </th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_utilizado,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_per_util_con,'2',',','.').' </th>';
            $html_dados_totais .= '<th>&nbsp;</th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_executado,'2',',','.').' </th>';
            $html_dados_totais .= '<th> '.number_format($total_vl_resultado,'2',',','.').' </th>';
            
            $html_dados_totais .= '</tr>';
            
            $html_dados .= $html_dados_totais;
        }
        $line_counter++;
    }
    
    $html_fim     = ' </table>
                     </body>
                    </html>';
    
    $html = $html.$html_titulos.$html_dados.$html_fim;
    
    return $html;
    
}

function getDadosRelatorioControleFinanceiro(){
    global $db;
    
    $strWhere = '';
    
    $filtros   = $_REQUEST;
    $obrid     = $filtros['obrid'];
    $arrMuncod = $filtros['slMunicipio'];
    $uf	       = $filtros['slEstado'];
    $nprocesso = $filtros['nprocesso'];
    $esfera    = $filtros['esfera'];
    
    if(!empty($obrid)){
        $strWhere .= " AND obr.obrid::text = '{$obrid}' " ;
    }
    
    if(!empty($esfera)){
        if($esfera == 'e'){
            $strWhere .= " AND (p.muncod is null or p.muncod = '') " ;
        }else{
            $strWhere .= " AND p.muncod is not null " ;
        }
    }
    
    if( (is_array($arrMuncod)) && (count($arrMuncod)) && ($arrMuncod[0]) ){
        foreach( $arrMuncod as $k => $v ){
            $arrMuncod[$k] = "'{$v}'";
        }			
        $strReturn = implode(",", $arrMuncod);
        $strWhere .= " AND m.muncod in ( $strReturn ) ";
    }
    
    if( (is_array($uf)) && (count($uf)) && ($uf[0]) ) {
        foreach( $uf as $k => $v ){
                $uf[$k] = "'{$v}'";
        }

        $strReturnUF = implode(",", $uf);
        $strWhere .= " AND 
                       CASE WHEN p.estuf IS NOT NULL THEN 

                            p.estuf in ( {$strReturnUF} )
                       ELSE 
                            m.estuf in ( {$strReturnUF} )
                       END ";
    }
    
    if(!empty($nprocesso)){
        $nprocesso = str_replace( '.' , '', $nprocesso);
        $nprocesso = str_replace( '/' , '', $nprocesso);
        $nprocesso = str_replace( '-' , '', $nprocesso);
        $strWhere .= " AND p.pronumeroprocesso ilike '%$nprocesso%' ";
    }
    
    $sql = "
            SELECT pronumeroprocesso,\"Número processo\", \"Termo de Compromisso\", \"ID da obra\", \"Nome Obra\", \"Tipologia\",
                   --Novas colunas:
                   \"UF\", \"Município\", \"Esfera\", \"situacao\",
                    \"Valor Contratado Obra\", \"Valor Contratado\", \"Valor Pactuado\", \"Valor da Parcela\", \"Saldo\", \"Referência do Saldo\",
                    --(\"Valor da Parcela\" - \"Saldo\") as \"Valor Utilizado\",
                    --(\"Valor Utilizado\" - \"Valor Contratado\") as \"% Utilizado do Contrato\",
                    --CASE WHEN \"Valor Contratado\" = 0 THEN 0.00 ELSE ((\"Valor da Parcela\" - \"Saldo\") / \"Valor Contratado\")*100 END as \"% Utilizado do Contrato\",
                    NULL as \"Valor Utilizado\",
                    NULL as \"% Utilizado do Contrato\",
                    \"% Executado\",
                    ((\"% Executado\" /100) * \"Valor Contratado\") as \"Valor Executado\",
                    --,(\"Valor Utilizado\" - \"Valor Executado\") as Resultado
                    --((\"Valor da Parcela\" - \"Saldo\") - (\"% Executado\" * \"Valor Contratado\")) as \"Resultado\"
                    NULL as \"Resultado\"
             FROM ( 
                     SELECT DISTINCT 
                             p.pronumeroprocesso,
                              pa.pagparcela as \"Parcela\",
                             to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as  \"Número processo\",
                             CASE WHEN em.prfid != 41 THEN
                                         cast ( (SELECT MAX(d.dopnumerodocumento)
                                             from par.documentopar  d
                                               inner join par.termocomposicao tc on tc.dopid = d.dopid
                                               inner join obras.preobra pre on pre.preid = tc.preid
                                               inner join obras2.obras o on o.preid = pre.preid
                                               WHERE pre.preid = obr.preid AND d.dopstatus = 'A'
                                               GROUP BY pre.preid) as varchar)
                                     ELSE
                                               cast ( (SELECT MAX(ter.terid || '/' || TO_CHAR(ter.terdatainclusao, 'YYYY'))
                                                     FROM par.termocompromissopac ter
                                                     LEFT JOIN par.termoobra tob ON tob.terid = ter.terid
                                                     WHERE tob.terid = obr.preid AND ter.terstatus='A') as varchar)
                                     END          as \"Termo de Compromisso\",
                             obr.obrid            as \"ID da obra\",-- id da obra;
                             oc.ocrvalorexecucao  as \"Valor Contratado Obra\",
                             crt.crtvalorexecucao  as \"Valor Contratado\",
                             obr.obrvalorprevisto as \"Valor Pactuado\",
                             ( SELECT SUM(popvalorpagamento) as perc_pag 
                             FROM par.pagamentoobrapar pao 
                             WHERE pao.pagid = pa.pagid 
                             AND pao.preid = poc.preid ) as \"Valor da Parcela\", -- Valor Repassado ?
                             --( SELECT COALESCE(sum(eo.eobvalorempenho),0.00) FROM par.empenho em INNER JOIN par.empenhoobra eo ON eo.empid = em.empid WHERE em.empnumeroprocesso = p.pronumeroprocesso AND eo.preid = pre.preid AND  em.empsituacao <> 'CANCELADO' and empstatus = 'A' and eobstatus = 'A' ) as \"Valor total empenhado da obra\", -- valor total empenhado;
                             (SELECT 
                              ( dfisaldoconta + dfisaldofundo + dfisaldopoupanca + dfisaldordbcdb) as \"Saldo\"
                              FROM painel.dadosfinanceirosconvenios
                              WHERE dfiprocesso = p.pronumeroprocesso
                              ORDER BY dfidatasaldo DESC
                              LIMIT 1) AS \"Saldo\",
                             (SELECT 
                              dfimesanosaldo AS \"Referência do Saldo\"
                              FROM painel.dadosfinanceirosconvenios
                              WHERE dfiprocesso = p.pronumeroprocesso
                              ORDER BY dfidatasaldo DESC
                              LIMIT 1) AS \"Referência do Saldo\",
                             --replace( obr.obrpercentultvistoria::text, '.',',') || '%' as \"% Executado\"
                              obr.obrpercentultvistoria as \"% Executado\",
                              
                              CASE WHEN (p.muncod is null or p.muncod = '') 
                              THEN 'Estadual' 
                              ELSE 'Municipal' 
                              END  as \"Esfera\",
                              m.mundescricao as \"Município\", -- município; 
                              m.estuf as \"UF\", -- uf; 
                              CASE WHEN esd.esdid = 693 
                              THEN '' || esd.esddsc || ''
                              WHEN esd.esdid = 769 
                              THEN '' || esd.esddsc || ''
                              ELSE esd.esddsc
                              END as \"situacao\",
                              tpo.tpodsc as \"Tipologia\",
                              obr.obrnome as \"Nome Obra\"

                     FROM par.pagamentoobrapar pob
                             INNER JOIN par.processoobrasparcomposicao poc ON poc.preid = pob.preid and poc.pocstatus = 'A'
                             INNER JOIN par.processoobraspar             p ON poc.proid = p.proid and p.prostatus = 'A'
                             LEFT JOIN painel.dadosfinanceirosconvenios dfi ON dfi.dfiprocesso = p.pronumeroprocesso
                             INNER JOIN par.empenho                    emp ON p.pronumeroprocesso = emp.empnumeroprocesso and emp.empsituacao <> 'CANCELADO' and empstatus = 'A'
                             LEFT JOIN par.pagamento                    pa ON pa.empid = emp.empid AND pob.pagid = pa.pagid AND pa.pagstatus = 'A'
                             INNER JOIN obras.preobra                  pre ON pre.preid = poc.preid
                             LEFT JOIN territorios.municipio             m ON m.muncod = pre.muncod
                             INNER JOIN obras.pretipoobra              pto ON pto.ptoid = pre.ptoid
                             LEFT JOIN obras2.obras                    obr ON obr.preid = pre.preid AND obr.obridpai IS NULL AND obr.obrstatus = 'A' --obr.obrid = pre.obrid_1
                             LEFT JOIN obras2.empreendimento            em ON obr.empid = em.empid
                             LEFT JOIN seguranca.usuario               usu ON usu.usucpf = pa.usucpf
                             LEFT join par.documentopar                 dp ON dp.proid  = p.proid AND dopstatus = 'A'
                             LEFT JOIN par.instrumentounidadeentidade  iue ON iue.inuid = p.inuid and iue.iuestatus = 'A' and iue.iuedefault = true
                             LEFT JOIN par.instrumentounidade           iu ON iu.inuid  = p.inuid
                             LEFT JOIN workflow.documento                d ON d.docid   = iu.docid AND d.tpdid = 44
                             LEFT JOIN workflow.estadodocumento         ed ON ed.esdid  = d.esdid
                             LEFT JOIN obras2.obrascontrato             oc ON oc.obrid  = obr.obrid AND obr.obrstatus = 'A' AND oc.ocrstatus = 'A'
                             LEFT JOIN obras2.contrato                 crt ON crt.crtid = oc.crtid
                             LEFT JOIN workflow.documento              doc ON doc.docid = obr.docid
                             LEFT JOIN workflow.estadodocumento        esd ON esd.esdid = doc.esdid
                             INNER JOIN obras2.tipologiaobra           tpo ON obr.tpoid = tpo.tpoid
                     WHERE (pa.pagsituacaopagamento = '2 - EFETIVADO'::text)
                       {$strWhere}

             UNION ALL

                     SELECT DISTINCT 
                             p.pronumeroprocesso,
                              pa.pagparcela as \"Parcela\",
                             to_char(p.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as  \"Número processo\", 
                             CASE WHEN em.prfid != 41 THEN
                                         cast ( (SELECT MAX(d.dopnumerodocumento)
                                             from par.documentopar  d
                                               inner join par.termocomposicao tc on tc.dopid = d.dopid
                                               inner join obras.preobra pre on pre.preid = tc.preid
                                               inner join obras2.obras o on o.preid = pre.preid
                                               WHERE pre.preid = obr.preid AND d.dopstatus = 'A'
                                               GROUP BY pre.preid) as varchar)
                                     ELSE
                                               cast ( (SELECT MAX(ter.terid || '/' || TO_CHAR(ter.terdatainclusao, 'YYYY'))
                                                     FROM par.termocompromissopac ter
                                                     LEFT JOIN par.termoobra tob ON tob.terid = ter.terid
                                                     WHERE tob.terid = obr.preid AND ter.terstatus='A') as varchar)
                                     END as \"Termo de Compromisso\",
                             obr.obrid as \"ID da obra\",-- id da obra;
                             oc.ocrvalorexecucao as \"Valor Contratado Obra\",
                             crt.crtvalorexecucao  as \"Valor Contratado\",
                             obr.obrvalorprevisto as \"Valor Pactuado\",
                             ( SELECT SUM(pobvalorpagamento) as perc_pag FROM par.pagamentoobra pao WHERE pao.pagid = pa.pagid AND pao.preid = poc.preid ) as \"Valor da Parcela\", -- Valor Repassado ?
                             --( SELECT COALESCE(sum(eo.eobvalorempenho),0.00) FROM par.empenho em INNER JOIN par.empenhoobra eo ON eo.empid = em.empid WHERE em.empnumeroprocesso = p.pronumeroprocesso AND eo.preid = pre.preid AND  em.empsituacao <> 'CANCELADO' and empstatus = 'A' and eobstatus = 'A' ) as \"Valor total empenhado da obra\",
                             (SELECT 
                              ( dfisaldoconta + dfisaldofundo + dfisaldopoupanca + dfisaldordbcdb) as \"Saldo\"
                              FROM painel.dadosfinanceirosconvenios
                              WHERE dfiprocesso = p.pronumeroprocesso
                              ORDER BY dfidatasaldo DESC
                              LIMIT 1) AS \"Saldo\",
                             (SELECT 
                              dfimesanosaldo AS \"Referência do Saldo\"
                              FROM painel.dadosfinanceirosconvenios
                              WHERE dfiprocesso = p.pronumeroprocesso
                              ORDER BY dfidatasaldo DESC
                              LIMIT 1) AS \"Referência do Saldo\",
                             --replace( obr.obrpercentultvistoria::text, '.',',') || '%' as \"% Executado\"
                              obr.obrpercentultvistoria as \"% Executado\",
                              
                              CASE WHEN (p.muncod is null or p.muncod = '') 
                              THEN 'Estadual' 
                              ELSE 'Municipal' 
                              END  as \"Esfera\",
                              m.mundescricao as \"Município\", -- município; 
                              m.estuf as \"UF\", -- uf; 
                              CASE WHEN esd.esdid = 693 
                              THEN '' || esd.esddsc || ''
                              WHEN esd.esdid = 769 
                              THEN '' || esd.esddsc || ''
                              ELSE esd.esddsc
                              END as \"situacao\",
                              tpo.tpodsc as \"Tipologia\",
                              obr.obrnome as \"Nome Obra\"

                     FROM par.pagamentoobra pob 
                             INNER JOIN par.processoobraspaccomposicao poc ON poc.preid = pob.preid and poc.pocstatus = 'A'
                             INNER JOIN par.processoobra                 p ON poc.proid = p.proid and p.prostatus = 'A'
                             LEFT JOIN painel.dadosfinanceirosconvenios dfi ON dfi.dfiprocesso = p.pronumeroprocesso
                             INNER JOIN par.empenho                    emp ON p.pronumeroprocesso = emp.empnumeroprocesso and emp.empsituacao <> 'CANCELADO' and empstatus = 'A' 
                             INNER JOIN par.pagamento                   pa ON pa.empid  = emp.empid AND pob.pagid = pa.pagid AND pa.pagstatus = 'A'
                             INNER JOIN obras.preobra                  pre ON pre.preid = poc.preid 
                             LEFT JOIN territorios.municipio             m ON m.muncod  = pre.muncod
                             inner join obras.pretipoobra              pto on pto.ptoid = pre.ptoid 
                             LEFT JOIN obras2.obras                    obr ON obr.preid = pre.preid AND obr.obridpai IS NULL AND obr.obrstatus = 'A' --obr.obrid = pre.obrid_1
                             LEFT JOIN obras2.empreendimento            em ON obr.empid = em.empid
                             LEFT JOIN seguranca.usuario               usu on usu.usucpf = pa.usucpf
                             LEFT JOIN par.termocompromissopac          tc ON tc.proid  = p.proid AND terstatus = 'A'
                             LEFT JOIN obras2.obrascontrato             oc ON oc.obrid  = obr.obrid AND obr.obrstatus = 'A' AND oc.ocrstatus = 'A'
                             LEFT JOIN obras2.contrato                 crt ON crt.crtid = oc.crtid
                             LEFT JOIN workflow.documento              doc ON doc.docid = obr.docid
                             LEFT JOIN workflow.estadodocumento        esd ON esd.esdid = doc.esdid
                             INNER JOIN obras2.tipologiaobra           tpo ON obr.tpoid = tpo.tpoid
                     WHERE (pa.pagsituacaopagamento = '2 - EFETIVADO'::text)
                       {$strWhere}
             ) as foo  
             ORDER BY 
              \"Número processo\", \"ID da obra\"

                        ";
                       
    try{
        $dados_brutos = $db->carregar($sql);        
    } catch (Exception $ex) {
        die('<pre><p>Erro ao fazer ao consulta:</p> '.$ex->getMessage().'</pre>');
    }               
    
    $dados_brutos = (!empty($dados_brutos)) ? $dados_brutos : array();
    
    //ver($sql,d);
//    ver($dados_brutos);
    
    $dados            = array();
    $obrid            = 0;
    $new_counter      = 0;
    $sum_repasse_fnde = 0;
    
    
    foreach ($dados_brutos as $key => $value) {
        if($key == 0){ $obrid = $value['ID da obra']; }
        
        if($obrid == $value['ID da obra']){
            $sum_repasse_fnde += $value['Valor da Parcela'];
            
            $dados[$new_counter]['pronumeroprocesso']       = $value['pronumeroprocesso'];
            $dados[$new_counter]['Número processo']         = $value['Número processo'];
            $dados[$new_counter]['Termo de Compromisso']    = $value['Termo de Compromisso'];
            $dados[$new_counter]['ID da obra']              = $obrid;
            $dados[$new_counter]['Nome Obra']               = $value['Nome Obra'];
            $dados[$new_counter]['Tipologia']               = $value['Tipologia'];
            $dados[$new_counter]['UF']                      = $value['UF'];
            $dados[$new_counter]['Município']               = $value['Município'];
            $dados[$new_counter]['Esfera']                  = $value['Esfera'];
            $dados[$new_counter]['situacao']                = $value['situacao'];
            $dados[$new_counter]['Valor Contratado Obra']        = $value['Valor Contratado Obra'];
            $dados[$new_counter]['Valor Contratado']        = $value['Valor Contratado'];
            $dados[$new_counter]['Valor Pactuado']          = $value['Valor Pactuado'];
            $dados[$new_counter]['Valor da Parcela']        = $sum_repasse_fnde;
            $dados[$new_counter]['Saldo']                   = $value['Saldo'];
            $dados[$new_counter]['Referência do Saldo']     = $value['Referência do Saldo'];
            $dados[$new_counter]['Valor Utilizado']         = '-';
            $dados[$new_counter]['% Utilizado do Contrato'] = '-';
            $dados[$new_counter]['% Executado']             = $value['% Executado'];
            $dados[$new_counter]['Valor Executado']         = $value['Valor Executado'];
            $dados[$new_counter]['Resultado']               = '-';
        }elseif($obrid != $value['ID da obra']){
            $sum_repasse_fnde  = 0;
            ++$new_counter;
            $obrid = $value['ID da obra'];
            $sum_repasse_fnde += $value['Valor da Parcela'];
            
            $dados[$new_counter]['pronumeroprocesso']       = $value['pronumeroprocesso'];
            $dados[$new_counter]['Número processo']         = $value['Número processo'];
            $dados[$new_counter]['Termo de Compromisso']    = $value['Termo de Compromisso'];
            $dados[$new_counter]['ID da obra']              = $obrid;
            $dados[$new_counter]['Nome Obra']               = $value['Nome Obra'];
            $dados[$new_counter]['Tipologia']               = $value['Tipologia'];
            $dados[$new_counter]['UF']                      = $value['UF'];
            $dados[$new_counter]['Município']               = $value['Município'];
            $dados[$new_counter]['Esfera']                  = $value['Esfera'];
            $dados[$new_counter]['situacao']                = $value['situacao'];
            $dados[$new_counter]['Valor Contratado Obra']        = $value['Valor Contratado Obra'];
            $dados[$new_counter]['Valor Contratado']        = $value['Valor Contratado'];
            $dados[$new_counter]['Valor Pactuado']          = $value['Valor Pactuado'];
            $dados[$new_counter]['Valor da Parcela']        = $sum_repasse_fnde;
            $dados[$new_counter]['Saldo']                   = $value['Saldo'];
            $dados[$new_counter]['Referência do Saldo']     = $value['Referência do Saldo'];
            $dados[$new_counter]['Valor Utilizado']         = '-';
            $dados[$new_counter]['% Utilizado do Contrato'] = '-';
            $dados[$new_counter]['% Executado']             = $value['% Executado'];
            $dados[$new_counter]['Valor Executado']         = $value['Valor Executado'];
            $dados[$new_counter]['Resultado']               = '-';
        }
        
        
    }
    
//    ver($dados);
    
    //Descomente essa linha para um detalhamento maior dos dados das obras
    //$dados = (!empty($dados_brutos)) ? $dados_brutos : array();
    
    return $dados;
}


?>