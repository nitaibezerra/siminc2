<?php
include_once APPRAIZ . "includes/workflow.php";
$inuid = $_REQUEST['inuid'] ? $_REQUEST['inuid'] : $_SESSION['par']['inuid'];

# Salvar escolas
function salvarEscolas(){
    global $db;

    $qtdMonitorada = pegaQtdMonitorada( $_REQUEST['icoid'] );

    if( $qtdMonitorada > 0 ){
        if( is_array($_POST['quantidade']) ){
            $qtd_atual = 0;
            foreach($_POST['quantidade'] as $qtd){
                $qtd_atual += $qtd;
            }
            if( $qtdMonitorada > $qtd_atual ){
                echo "
                    <script>
                        alert('Aquantidade deve ser maior ou igual a $qtdMonitorada.');
                        window.location.href = window.location.href;
                    </script>";
                die();
            }
        }
    }
	
    $sql = "";
    $n = 0;
    $erro = false;
	
    if( is_array($_POST['sesid']) ){
        if( is_array($_POST['quantidade']) ){
            foreach($_POST['sesid'] as $v => $valor){
                $quantidade = $_POST['quantidade'][$v] ? $_POST['quantidade'][$v] : 0;
                $quantidade = str_replace(".", "", $quantidade);
                if( !is_numeric( $quantidade ) ){
                    $erro = true;
                }

                if( $_GET['tipo'] == 'tec' ){
                    $str = "AND seiqtdtecnico IS NOT NULL";
                    $ins = "seiqtdtecnico";

                    $sql .= "UPDATE par.subescolas_subitenscomposicao SET seiqtdtecnico = {$quantidade} WHERE sesid = {$valor} AND icoid = ".$_REQUEST['icoid'] ." ; ";
                    $n++;
                } else {
                    $sql2 = "SELECT sesid FROM par.subescolas_subitenscomposicao WHERE sesid = {$valor} AND icoid = {$_REQUEST['icoid']} ";
                    $sesid = $db->pegaUm($sql2);
                    if( $sesid ){
                        $sql .= "DELETE FROM par.subescolas_subitenscomposicao WHERE sesid = {$valor} AND icoid = {$_REQUEST['icoid']}; ";
                    }

                    $icosql = "SELECT count(1) FROM par.subacaoitenscomposicao WHERE icoid = ".$_REQUEST['icoid'];
                    if( $db->pegaUm( $icosql ) > 0 ){
                        $sql .= "INSERT INTO par.subescolas_subitenscomposicao (sesid, icoid, seiqtd) VALUES ( {$valor}, {$_REQUEST['icoid']}, {$quantidade} ); ";
                        $n++;
                    }
                }
            }
        }
    }

    if( $erro == true ){
        echo "<script>
                alert('Problema na execução! O campo só aceita números!\\nDigite o número em vez de colá-lo com o mouse!');
                window.location.href = window.location;
            </script>";	
    } else {
        if( $n != 0 ){
            if($db->executar( $sql )){
                $db->commit();
                
                $sbaid = $_REQUEST['sbaid'];
                $sbdano = $_REQUEST['ano'];
                if($sbaid && $sbdano)
                {
                	
                	
                	atualizaValorSubacaoDetalhe($sbaid, $sbdano);
                }
                echo "<script>
                        alert('Operação realizada com sucesso!');
                        parent.window.opener.location.href = parent.window.opener.location;
                        window.close();
                    </script>";	
            } else {
                $db->rollback();
                echo "<script>
                        alert('Problema na execução!');
                        window.close();
                    </script>";
            }
        } else {
            echo "<script>
                    alert('Preencha com algum valor!');
                    window.close();
                </script>";
        }
    }
    exit;
}

#REQUISICAO
if($_POST['requisicao']){
    $_POST['requisicao']();
}

#BUSCA ITRID
$sbaid = $_REQUEST['sbaid'];
$itrid = pegaItridSubacao( $sbaid );
if(!$sbaid){
    echo "<script>alert('Subação não encontrada!');window.close();</script>";
}

#EXPORTAR
if (isset($_REQUEST['exportar'])) {
     ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_RelatEscola" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_RelatEscola" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");
        echo $_REQUEST['htmlExcel'];
        exit;
}

#QUANTIDADE MONITORADA
$qtdMonitorada = pegaQtdMonitorada( $_REQUEST['icoid'] );

if( $qtdMonitorada > 0 ):
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr style="background-color: #cccccc;">
        <td align="center" colspan="2" style="color:red"><strong>Item encontra-se em monitoramento.</strong></td>
    </tr>
    <tr>
        <td align="center" colspan="2" ><strong>Quantidade monitorada: <?php echo $qtdMonitorada ?></strong></td>
    </tr>
    <tr>
        <td align="center" colspan="2" ><strong>Quantidade atual: <label id="qtd_atual"></label></strong></td>
    </tr>
</table>
<?php endif; ?>
<html>
    <head>
        <meta http-equiv="Cache-Control" content="no-cache">
        <meta http-equiv="Pragma" content="no-cache">
        <meta http-equiv="Connection" content="Keep-Alive">
        <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
        <title>PAR 2010 - Plano de Metas - Subação</title>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
        <link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
    </head>
    <body>
        <script type="text/javascript">
            $(document).ready(function(){
                <?php if( $qtdMonitorada > 0 ){?>
                var qtd = 0;
                $('[name="quantidade[]"]').each(function(){
                    if( parseFloat($(this).val()) > 0 ){
                        qtd = parseFloat(qtd) + parseFloat($(this).val());
                    }
                });
                $('#qtd_atual').html(qtd);
                $('[name="quantidade[]"],[name="quantidadePadrao"]').blur(function(){
                    qtd = 0;
                    $('[name="quantidade[]"]').each(function(){
                        if( parseFloat($(this).val()) > 0 ){
                            qtd = parseFloat(qtd) + parseFloat($(this).val());
                        }
                    });
                    if( qtd < <?php echo $qtdMonitorada ?> ){
                        $('#qtd_atual').css('color','red');
                    }else{
                        $('#qtd_atual').css('color','black');
                    }
                    $('#qtd_atual').html(qtd);
                });
                <?php }?>
            });

            function salvarEscolas()
            {
                var erro = 0;
                $("[class~=obrigatorio]").each(function() { 
                    if(!this.value || this.value === "Selecione..."){
                        erro = 1;
                        alert('Favor preencher todos os campos obrigatórios!');
                        this.focus();
                        return false;
                    }
                });
                <?php if( $qtdMonitorada > 0 ){?>
                var qtd = 0;
                $('[name="quantidade[]"]').each(function(){
                    if( parseFloat($(this).val()) > 0 ){
                        qtd = parseFloat(qtd) + parseFloat($(this).val());
                    }
                });
                if( qtd < <?php echo $qtdMonitorada ?> ){
                    alert('Aquantidade deve ser maior ou igual a <?php echo $qtdMonitorada ?>.');
                    return false;
                }
                <?php }?>
                if(erro == 0){
                    $("#form_subacao").submit();
                }
            }

            function preencherTodasQtds( valor ){
                var qtdPadrao = valor;
                var arInputQtd = document.getElementsByName('quantidade[]');

                if( qtdPadrao ){
                    for( i = 0; i < arInputQtd.length; i++ ){
                        arInputQtd[i].value = qtdPadrao;
                    }
                }
            }  
            
            function exportarExcel(){
                var html = '<table style="width:100%; border 1px solid #F5F5F5">';
                html += $('.listagem').html();
                html += '<tbody>';
                $('#resultadoEscola tbody').children('tr').each(function (){
                  if (!$(this).hasClass('vlrPadrao')) {
                    html += '<tr>';
                    $(this).children('td').each(function (){
                        html += '<td>';
                        if ($(this).children('input').length !== 0) {
                            html += $(this).children('input').val();
                        } else {
                            html += $(this).html();
                        }
                        html += '</td>';
                    });
                    html += '</tr>';
                  }
                });
                html += '</tbody>';
                html += '</table>';
               $('input[name=htmlExcel]').val(html);
               $('#formExportar').submit();
            }
        </script>
        <form name="form_subacao" id="form_subacao" method="post" action="" >
            <input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
            <input type="hidden" name="requisicao" value="salvarEscolas" />
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr style="background-color: #cccccc;">
                    <td align="center" colspan="2"><strong>Escolas</strong></td>
                </tr>
                <tr>
                    <td colspan="2" id="tblResultadoEscola">
                        <?php 
                        # Se a consulta for de qtd tecnico concatena tecnico a coluna seiqtd.
                        $tipoConsulta = ($_GET['tipo'] == 'tec') ? 'tecnico' : '';
                        //Estadual
                        if( $itrid == 1 ) {
                            $cols = 4;
                            $arrTamTd = array(27,20,8,10,15,10,10);
                            $cabecalho = array("Município","Escola","Código INEP","Tipo da escola","Quantidade","Quantidade de Salas","Quantidade de alunos");

                            $sql = " SELECT DISTINCT
                                        m.mundescricao,
                                        t.entnome,
                                        t.entcodent,
                                        e.esctipolocalizacao as escola_localizacao,
                                        ssi.seiqtd{$tipoConsulta} as quantidade,
                                        e.escqtdsalas as escola_qtd_salas,
                                        e.escqtdalunos as escola_qtd_alunos,
                                        ses.sesid
                                    FROM
                                        entidade.entidade t
                                    INNER JOIN entidade.funcaoentidade f ON f.entid = t.entid
                                    LEFT JOIN entidade.entidadedetalhe ed ON t.entid = ed.entid
                                    INNER JOIN entidade.endereco d ON t.entid = d.entid
                                    LEFT JOIN territorios.municipio m ON m.muncod = d.muncod
                                    LEFT JOIN par.escolas e ON e.entid = t.entid
                                    INNER JOIN par.subacaoescolas ses ON ses.escid = e.escid 
                                        AND ses.sbaid = '{$_GET['sbaid']}' 
                                        AND ses.sesstatus = 'A' 
                                        AND ses.sesano = '{$_GET['ano']}'
                                    LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = ses.sesid 
                                        AND ssi.icoid = {$_GET['icoid']}
                                        AND ses.sesano = '{$_GET['ano']}'
                                        AND ses.sbaid = '{$_GET['sbaid']}'
                                    WHERE (t.entescolanova = false or t.entescolanova is null)
                                    AND t.entstatus = 'A'
                                    AND f.funid = 3
                                    AND t.tpcid = 1
                                    AND m.estuf = '{$_SESSION['par']['estuf']}'
                                    ORDER BY
                                        m.mundescricao,
                                        t.entnome"; 
                        } else {
                            $arrTamTd = array(24,24,10,15,13,14);
                            $cols = 3; 
                            $cabecalho = array("Escola","Código INEP","Tipo da escola","Quantidade","Quantidade de Salas","Quantidade de alunos");

                            $tcpid = 3;
                            $whereMunCod = " AND d.muncod = '{$_SESSION['par']['muncod']}'";
                            if ($_SESSION['par']['muncod'] == '5300108' || $_SESSION['par']['estuf']=='DF') {
                                $tcpid = 1;
                                $whereMunCod = "";
                            }

                            $sql = "SELECT DISTINCT
                                        t.entnome,
                                        t.entcodent,
                                        e.esctipolocalizacao as escola_localizacao,
                                        seiqtd{$tipoConsulta} as quantidade,
                                        e.escqtdsalas as escola_qtd_salas,
                                        e.escqtdalunos as escola_qtd_alunos,
                                        ses.sesid
                                    FROM
                                        entidade.entidade t
                                    INNER JOIN entidade.funcaoentidade f on f.entid = t.entid
                                    LEFT JOIN entidade.entidadedetalhe ed on t.entid = ed.entid
                                        AND (
                                            entdreg_infantil_creche = '1' or
                                            entdreg_infantil_preescola = '1' or
                                            entdreg_fund_8_anos = '1' or
                                            entdreg_fund_9_anos = '1'
                                        )
                                    INNER JOIN entidade.endereco d on t.entid = d.entid
                                    LEFT JOIN par.escolas e on e.entid = t.entid
                                    INNER JOIN par.subacaoescolas ses on ses.escid = e.escid 
                                        AND ses.sesstatus = 'A'
                                        AND ses.sesano = '{$_GET['ano']}'
                                        AND ses.sbaid = '{$_GET['sbaid']}'
                                    LEFT JOIN par.subescolas_subitenscomposicao ssc ON ssc.sesid = ses.sesid 
                                        AND ssc.icoid = {$_GET['icoid']}
                                    WHERE (t.entescolanova = false or t.entescolanova is null)
                                    AND t.entstatus = 'A'
                                    AND f.funid = 3
                                    AND t.tpcid = {$tcpid}
                                    {$whereMunCod}
                                    ORDER BY t.entnome";
                        }

                        $docidSubacao = $db->pegaUm("SELECT docid FROM par.subacao WHERE sbaid = ".$sbaid);
                        $estadoAtual = wf_pegarEstadoAtual( $docidSubacao );
                        $perfil = pegaArrayPerfil($_SESSION['usucpf']);

                        $sqlRef = "SELECT sbareformulacao FROM par.subacao WHERE sbaid = ".$sbaid;
                        $reformulacao = $db->pegaUm( $sqlRef );

                        if(in_array(PAR_PERFIL_EQUIPE_FINANCEIRA ,$perfil)){
                                $permitido = verifcaMunEstUsusario(PAR_PERFIL_EQUIPE_FINANCEIRA);
                        }elseif	(in_array(PAR_PERFIL_EQUIPE_TECNICA ,$perfil)){
                                $permitido = verifcaMunEstUsusario(PAR_PERFIL_EQUIPE_TECNICA);
                        }

                        if( ( in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ) ||
                        ( $permitido == 'S' && ( $estadoAtual['esdid'] == WF_SUBACAO_ANALISE || $reformulacao == 't' ) && ( in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $perfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil) ) ) ||
                        ( ( $estadoAtual['esdid'] == WF_SUBACAO_ELABORACAO || $estadoAtual['esdid'] == WF_SUBACAO_DILIGENCIA || $reformulacao == 't') && ( in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$perfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$perfil) ) ) ||
                        ( ( $estadoAtual['esdid'] == WF_SUBACAO_ELABORACAO || $estadoAtual['esdid'] == WF_SUBACAO_DILIGENCIA || $reformulacao == 't') && ( in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$perfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$perfil) || in_array(PAR_PERFIL_PREFEITO,$perfil) ) ) ||
                        ( ( $estadoAtual['esdid'] == WF_SUBACAO_ELABORACAO || $estadoAtual['esdid'] == WF_SUBACAO_DILIGENCIA || $reformulacao == 't') && ( in_array(PAR_PERFIL_COORDENADOR_GERAL ,$perfil) ) )
                        ){
                                $habilitado = 'S';
                        }else{
                                $habilitado = 'N';
                        }	

                        # SE ESTÁ EM ATUALIZAÇÃO
                        if( $estadoAtual['esdid'] == WF_SUBACAO_ATUALIZACAO ){
                            if(	in_array(PAR_PERFIL_PREFEITO,$perfil) ||
                                in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$perfil) ||
                                in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$perfil) ||
                                in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$perfil) ||
                                in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$perfil) ||
                                in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
                                in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)
                            ){
                                if( $_REQUEST['ano'] >= date("Y") ){
                                    $habilitado = 'S';
                                }
                            }
                        }

                        if( $_REQUEST['sbaid'] ){
                            $sqlTeste = "SELECT
                                            dpr.sbdid
                                        FROM
                                            par.documentoparreprogramacaosubacao dpr
                                        INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = dpr.sbdid
                                        WHERE
                                            dpr.dpsstatus = 'P'
                                            AND sbd.sbaid = {$_REQUEST['sbaid']}
                                            AND sbd.sbdano = '{$_REQUEST['ano']}'";

                            $reprogramacao = $db->pegaUm($sqlTeste);
                            if($reprogramacao){
                                $habilitado = 'N';
                            }
                        }
                        
                        $arrEscolas = $db->carregar($sql);
                        if($arrEscolas): ?>
                            <table class="listagem" width="100%" >
                                <thead>
                                    <tr class="SubTituloTabela">
                                        <?php foreach($cabecalho as $chave => $cab): ?>
                                            <td width="<?php echo $arrTamTd[$chave] ?>%" class="bold" ><?php echo $cab ?></td>
                                        <?php endforeach; ?>
                                    </tr>
                                </thead>
                            </table>
                            <div id="tbl_escolas" >
                                <table class="listagem" id="resultadoEscola" width="100%" >
                                    <?php 
                                    if( count($arrEscolas) > 17){
                                        $arrTamTd[count($arrTamTd)-1] = $arrTamTd[count($arrTamTd)-1] - 3 ;
                                    }
                                    if(   $_REQUEST['visualizacao'] != 'visu' ){
                                    ?>
                                    <tr bgcolor="#f7f7f7" class="vlrPadrao">
                                        <td colspan="<?php echo $cols; ?>"></td>
                                        <td width="<?php echo $arrTamTd[$escNum] ?>%" align="center" ><?php echo campo_texto("quantidadePadrao","N",$habilitado,"",10,12,"[.###]","","right","","","","", ($esc ? number_format($esc,0,2,'.') : ""),"preencherTodasQtds( this.value );" ) ?><br>Qtd Padrão</td>
                                    </tr>										
                                    <?php }
                                    $num = 0; ?>
                                    <?php foreach($arrEscolas as $escola): ?>
                                            <?php $cor = $num%2 == 1 ? "#f7f7f7" : "#FFFFFF"?>
                                            <tr bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" >
                                            <?php $escNum = 0; ?>
                                            <?php foreach($escola as $chave => $esc): ?>
                                                    <?php if($chave == 'sesid'){ ?>
                                                        <input type="hidden" name="sesid[]" id="sisid[]" value="<?php echo $escola['sesid']?>">
                                                    <?php } else { ?>
                                                    <td width="<?php echo $arrTamTd[$escNum] ?>%" align="<?php echo $chave != "entnome" ? "center" : "left" ?>" >
                                                        <?php 
                                                        if( ( 	in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$perfil) || 
																in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$perfil) || 
																in_array(PAR_PERFIL_PREFEITO,$perfil) 
															) && ($_REQUEST['visualizacao'] == 'visu') ){
																echo  $chave == "quantidade" ? number_format($esc,0,2,'.')  : $esc;
                                                        	}
                                                        	else
                                                        	{
                                                        		echo  $chave == "quantidade" ? campo_texto("quantidade[]","N", $habilitado,"",10,12,"[.###]","","right","","","","", ($esc ? number_format($esc,0,2,'.') : "") ) : $esc ;
                                                        	}
                                                        ?>
                                                    </td>
                                                    <?php } ?>
                                                    <?php $escNum++; ?>
                                            <?php endforeach; ?>
                                            </tr>
                                            <?php $num++; ?>
                                    <?php endforeach; ?>
                                </table>
                            </div>
                        <?php endif; ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita"></td>
                    <td class="SubTituloEsquerda" >
                        <?php
                        if($habilitado == 'S'){
                        ?>
                            <input type="button" value="Salvar" name="btn_salvar" onclick="salvarEscolas()" />
                        <?php }?>
                        <input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
                        <input type="button" value="Exportar" name="btn_exportar" onclick="exportarExcel()" />
                    </td>
                </tr>
            </table>
        </form>
        <form name="formExportar" id="formExportar" method="post" action="" >
            <input type="hidden" name="htmlExcel" value="" />
            <input type="hidden" name="exportar" value="S" />
        </form>
    </body>
</html>