<?PHP
    #VERIFICA SE EXISTE A SESSÃO. "USUARIO SELECIONADO" CASO NÃO, VIRIFICA QUAL O PERFIL E REDIRECIONA PARA A RESPECTIVA PAGINA.
    $perfil = pegaPerfilGeral();

    if( !(in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ) ){
        if($_SESSION['cpfavaliado'] == ''){
            $db->sucesso( 'principal/avaliacao_servidor/lista_grid_pess_avaliados', '', 'Não é possivel realizar a Avaliação. Selecione um servidor!' );
            die();
        }
    }else{
        if($_SESSION['cpfavaliado'] == ''){
            $db->sucesso( 'principal/avaliacao_servidor/lista_grid_servidores_avaliados', '', 'Não é possivel realizar a Avaliação. Selecione um servidor!' );
            die();
        }
    }

    #VERIFICA SE O USUÁRIO TEM CHEFIA CASO NÃO, NÃO É ABERTO A TELA E É EXIBIDA UM MSG.
    verificaSeUsuarioTemChefia( $_SESSION['cpfavaliado'] );
    
    if( $_POST['action'] == 'salvar'){
        salvar();
    }

    if( $_POST['req'] == 'limparAv' ){
        $sql = "DELETE FROM gestaopessoa.respostaavaliacao WHERE sercpf = '".$_POST['cpfavaliado']."' AND resano = {$_SESSION['exercicio']}";
        $db->executar($sql);

        $sql = "DELETE FROM gestaopessoa.respostaavaliacao WHERE resavaliacpf = '".$_POST['cpfavaliado']."' AND resano = {$_SESSION['exercicio']}";
        $db->executar($sql);
        
        $sql = " UPDATE gestaopessoa.servidor SET serlimpaaval = 'f' WHERE sercpf = '{$_SESSION['cpfavaliado']}' AND seranoreferencia = {$_SESSION['exercicio']} RETURNING sercpf;";
        $db->executar($sql);

        $db->commit();
    }

    #VERIFICA SE A PERMISSAO PARA SER FEITA A AVALIAÇÃO. CASO NÃO RETORNA O USUARIO PARA A PAGINA DE LISTAGEM. (não é usado mais)
    /*
    if( !in_array( PERFIL_SUPER_USER, $perfil ) ){

        $data_hoje      = Date("c");
        $data_fechamento= DATA_DE_FECHAMENTO_AVALIACAO;

        if( strtotime($data_hoje) >= strtotime($data_fechamento) ){
            if( !in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil )){
                $db->sucesso('principal/avaliacao_servidor/lista_grid_pess_avaliados', '', 'Data para Avaliação foi encerrada!');
                exit();
            }else{
                $db->sucesso('principal/avaliacao_servidor/lista_grid_servidores_avaliacao', '', 'Data para Avaliação foi encerrada!');
                exit();
            }
        }
    }
    */

    function limparNotas($tavid){
        global $db;
        $sql = "
            DELETE FROM gestaopessoa.respostaavaliacao
            WHERE sercpf = '{$_SESSION['cpfavaliado']}' AND tavid = {$tavid} AND resano = {$_SESSION['exercicio']}
        ";
        $db->executar( $sql );
        $db->commit();
    }

    function salvar(){
        global $db;

        $sqlChefe = "select sercpf from gestaopessoa.servidor where sercpfchefe = '".$_SESSION['usucpf']."' AND sercpf = '".$_SESSION['cpfavaliado']."' AND seranoreferencia = '{$_SESSION['exercicio']}'";
        $chefe = $db->pegaUm( $sqlChefe );

        if( $chefe ){
            $tavid = 2;
        } elseif( $_SESSION['cpfavaliado'] == $_SESSION['usucpf'] ){
            $tavid = 1;
        } else {
            $tavid = 3;
        }

        if( trim( $_POST['pendente'] ) == "gravar"){
                $pendente = "t";

                if( is_array( $_POST['defid'] ) ){

                    foreach( $_POST['defid'] as $defid => $resnota ){
                        if( is_array( $resnota ) ){
                            $res = $resnota[1];
                        }else{
                            $res = $resnota;
                        }
                        $res = $res ? $res : 0;

                        $sql = "
                            INSERT INTO gestaopessoa.respostaavaliacao(
                                    tavid, defid, sercpf, resnota, resano, resavaliacaopendente, resavaliacpf
                                ) VALUES (
                                    {$tavid}, {$defid}, '{$_SESSION['cpfavaliado']}', '{$res}', {$_SESSION['exercicio']}, '{$pendente}', '{$_SESSION['usucpf']}'
                            );
                        ";
                        $ins = $db->executar( $sql );

                        if( $tavid == 2 ){

                            $sql_f = "
                                SELECT  trim(sercodigofuncao),
                                        sernivelfuncao
                                FROM gestaopessoa.servidor
                                WHERE sercpf = '{$_SESSION['usucpf']}' AND seranoreferencia = {$_SESSION['exercicio']}
                            ";
                            $funcao = $db->carregar($sql_f);

                            if( $funcao[0]['sercodigofuncao'] == 'DAS' && $funcao[0]['sernivelfuncao'] != 1014 ){
                                    $sql = "
                                        INSERT INTO gestaopessoa.respostaavaliacao(
                                                tavid, defid, sercpf, resnota, resano, resavaliacaopendente, resavaliacpf
                                            ) VALUES (
                                                3, {$defid}, '{$_SESSION['cpfavaliado']}', '{$res}', {$_SESSION['exercicio']}, '{$pendente}', '{$_SESSION['usucpf']}'
                                        );
                                    ";
                                    $ins = $db->executar( $sql );
                            }
                        }
                    }
                    $db->commit();
                }
        }elseif( trim( $_POST['pendente'] ) == "finalizar" ){
            $pendente = "f";

            $sql = "
                UPDATE gestaopessoa.respostaavaliacao
                        SET resavaliacaopendente = '{$pendente}'
                WHERE sercpf = '{$_SESSION['cpfavaliado']}' AND resano = {$_SESSION['exercicio']}
            ";
            $db->executar( $sql );
            $db->commit();
        }
        echo("<script>alert('Operação realizada com sucesso.')\n</script>");
        echo("<script>window.location.href = 'gestaopessoa.php?modulo=principal/avaliacao_servidor/cad_avaliacao_servidor&acao=A';</script>");
        exit();
    }

    if( $_SESSION['cpfavaliado'] != '' ){
        $dados = buscaDadosServidorAvaliacao( $_SESSION['cpfavaliado'] );
    }

    include  APPRAIZ."includes/cabecalho.inc";

    monta_titulo( 'Gestão de Pessoas', 'Formulario de Avaliação' );
    echo "<br>";

    #VARIAVEL E CONTROLE DE ABAS.
    if( in_array( PERFIL_SUPER_USER, $perfil ) || in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) ){
        $abacod_tela = ABA_CAD_AVALIACAO_SERVIDOR_ADM;
    }else{
        $abacod_tela = ABA_CAD_AVALIACAO_SERVIDOR;
    }

    $url        = 'gestaopessoa.php?modulo=principal/avaliacao_servidor/cad_avaliacao_servidor&acao=A';
    $parametros = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

?>
    <header>
        <style type="text/css">
            .textoJustificado{
                font-weight: normal;
                font-size: 11px;
                color: black;
                font-family: Arial, Verdana;
                background-color: #f0f0f0;
                text-align: justify;
                line-height: 19px;
            }
        </style>

        <script type="text/javascript" src="/includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

    </header>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td colspan="4" class="subTituloCentro" style="font-size: 15px; background-color: #dcdcdc;">
                Formulário de Avaliação de Desempenho Individual - <?= $dados['sergratificacao'] ? $dados['sergratificacao'] : 'TIPO DE GRATIFICAÇÃO NÃO INFORMADO';?>
            </td>
        </tr>
        <tr>
            <td colspan="4" class="subtituloEsquerda" width="15%" style="font-size: 14px; background-color: #e9e9e9;">Instruções</td>
        </tr>
        <tr>
            <td colspan="4" class="textoJustificado">
                <p>
                    A premissa básica deste Instrumento de Avaliação é a de que o avaliado e o avaliador sejam capazes de realizar um
                    exercício de maturidade profissional e respeito mútuo, cujo resultado seja uma Avaliação Consensual, fruto de um
                    diálogo franco e responsável. Procure desfrutar intensamente este momento, transformando-o em uma demonstração de
                    abertura, aprendizagem e auto desenvolvimento. O servidor será avaliado em cada um dos Fatores indicados no
                    bloco 3 abaixo, que representam aspectos observáveis do desempenho e referem-se ao trabalho efetivamente realizado
                    pelo servidor, podendo a avaliação variar de 000 a 100, sendo multiplicado pelo seu respectivo peso para definição
                    da nota final.
                </p>
            </td>
        </tr>
        <tr>
            <td colspan="4" class="subtituloEsquerda" width="15%" style="font-size: 15px; background-color: #e9e9e9;">Identificação</td>
        </tr>
        <tr>
            <td class="subTituloDireita" width="30%">Servidor:</td>
            <td>
                <?php
                    $sernome = $dados['sernome'];
                    echo campo_texto('sernome', 'N', 'N', '', 50, 14, '', '', '', '', 0, 'id="sernome"', '', $sernome, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subTituloDireita">SIAPE:</td>
            <td>
                <?php
                    $sersiape = $dados['sersiape'];
                    echo campo_texto('sersiape', 'N', 'N', '', 50, 14, '', '', '', '', 0, 'id="sersiape"', '', $sersiape, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subTituloDireita">Cargo Efetivo:</td>
            <td>
                <?php
                    $sercargo = $dados['sercargo'];
                    echo campo_texto('sercargo', 'N', 'N', '', 50, 14, '', '', '', '', 0, 'id="sercargo"', '', $sercargo, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subTituloDireita">Chefe:</td>
            <td>
                <?php
                    $serdescricao_chefe = $dados['serdescricao_chefe'];
                    echo campo_texto('serdescricao_chefe', 'N', 'N', '', 50, 14, '', '', '', '', 0, 'id="serdescricao_chefe"', '', $serdescricao_chefe, '', null);
                ?>
            </td>
        </tr>
    </table>

    <table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
<?PHP

    $sqlChefe = "select sercpf from gestaopessoa.servidor where sercpfchefe = '".$_SESSION['usucpf']."' AND sercpf = '".$_SESSION['cpfavaliado']."' AND seranoreferencia = '{$_SESSION['exercicio']}'";
    $chefe = $db->pegaUm( $sqlChefe );

    if( $chefe ){
        $tavid = 2;
    } elseif( $_SESSION['cpfavaliado'] == $_SESSION['usucpf'] ){
        $tavid = 1;
    } else {
        $tavid = 3;
    }

    $sql = "
        SELECT  resid, resavaliacaopendente
        FROM gestaopessoa.respostaavaliacao
        WHERE resavaliacpf = '{$_SESSION['usucpf']}' AND sercpf = '{$_SESSION['cpfavaliado']}' AND resano = {$_SESSION['exercicio']} AND tavid = {$tavid}
    ";
    $res = $db->pegaLinha( $sql );

    if( $res['resavaliacaopendente'] == '' ){
        $sql = "
            SELECT resavaliacaopendente
            FROM gestaopessoa.respostaavaliacao
            WHERE sercpf = '{$_SESSION['cpfavaliado']}' AND resano = {$_SESSION['exercicio']}
        ";
        $avaliacao = $db->pegaUm( $sql );
    }else{
        $avaliacao = $res['resavaliacaopendente'];
    }

    $resposta = $res['resid'];//notas
    $mascaraGlobalJs = "this.value=mascaraglobal('###',this.value);";

    $resposta_resid = verificaSeHaResposta();

    #BUSCA O TIPO DE GRATIFICAÇÃO QUE É ATRIBUIDA AO SERVIDOR AVALIADO.
    $resp = verificaTipoGratificacaoServidor( $_SESSION['cpfavaliado'] );

    #VERIFICA SE O USUÁRIO LOGADO É O MESMO A SER AVALIADO E SE SUA GRATIFICAÇÃO E DE CEDIDO OU GDAPS. CASO SEJA NÃO É POSSÍVEL FAZER A AVALIAÇÃO.
    if( ( $resp['tssid'] == 8 || $resp['sertipogratificacao'] == 'PS' ) && ( $_SESSION['usucpf'] == $_SESSION['cpfavaliado'] ) ){
        $perm = 'N';
    }else{
        $perm = 'S';
    }
?>
    <br>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td  colspan="4" class="subtituloEsquerda" width="15%" style="font-size: 15px; background-color: #e9e9e9;"> Fatores de Avaliação </td>
        </tr>
    </table>

    <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
        <input type="hidden" id="action" name="action" value="0">
        <input type="hidden" id="ano_exercicio" name="ano_exercicio" value="<?=$_SESSION['exercicio'] ?>">
        <input type="hidden" id="ano_atual" name="ano_atual" value="<?=date('Y'); ?>">
        <input type="hidden" id="pendente" name="pendente" value="0">

        <table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
        <tr>
            <td style="padding:5px; background-color:#B5B5B5; color:#404040; font-size: 12px; font-weight: bold; text-align: center;">Competência</td>
            <td style="padding:5px; background-color:#B5B5B5; color:#404040; font-size: 12px; font-weight: bold; text-align: center;">Definição</td>

    <?PHP
        if( ( in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ) || ( $resp['tssid'] == 8 ) && ( $resposta_resid > 0 ) ){
            echo getAvaliadorHTML('TIPO_CABECALHO');
        }else{
            if( !$resposta && ($avaliacao || $avaliacao == '') ){
    ?>
                <td style="padding:5px; background-color:#B5B5B5; color:#404040; font-size: 12px; font-weight: bold; text-align: center;">Avaliação</td>
                <td style="padding:5px; background-color:#B5B5B5; color:#404040; font-size: 12px; font-weight: bold; text-align: center;">Total</td>
    <?PHP
            }else{
                echo getAvaliadorHTML('TIPO_CABECALHO');
            }
        }
    ?>
        </tr>

    <?PHP
        switch( $resp['sertipogratificacao'] ){
            case 'PE':
                $and = "AND defgdpgpe = 't'";
            break;
            case 'CE':
                $and = "AND defgdace = 't'";
            break;
            case 'PS':
                $and = "AND defgdaps = 't'";
            break;
        }
        
        if( $resp['tssid'] ==  8 ){
            $and = "AND defgtrcedido = 't'";
        }

        $sql = "
            SELECT  d.defid,
                    d.defdescricao,
                    d.defpeso,
                    d.defpesoaa,
                    d.defpesoc,
                    c.comdescricao
            FROM gestaopessoa.definicao AS d
            INNER JOIN gestaopessoa.competencia AS c ON c.comid = d.comid
            INNER JOIN gestaopessoa.avaliacao ON avaanoreferencia = {$_SESSION["exercicio"]}
            WHERE d.defanoreferencia = {$_SESSION["exercicio"]} AND c.comano = {$_SESSION["exercicio"]} {$and}
        ";
        $rsDados = $db->carregar( $sql );

        $sqlMa = "
            SELECT  ROUND(AVG(resnota))
                    --ROUND(count(resnota))
            FROM gestaopessoa.respostaavaliacao
            WHERE sercpf = '{$_SESSION['cpfavaliado']}' AND tavid = ".TIPO_AUTO_AVAL." --AND resano = {$_SESSION['exercicio']}
        ";
        $mediaMA = $db->pegaUm( $sqlMa );

        $sqlMs = "
            SELECT  ROUND(AVG(resnota))
                    --ROUND(count(resnota))
            FROM gestaopessoa.respostaavaliacao
            WHERE sercpf = '{$_SESSION['cpfavaliado']}' AND tavid = ".TIPO_AVAL_SUPERIOR." --AND resano = {$_SESSION['exercicio']}
        ";
        $mediaMS = $db->pegaUm( $sqlMs );

        $sqlMC = "
            SELECT  ROUND(AVG(resnota))
                    --ROUND(count(resnota))
            FROM gestaopessoa.respostaavaliacao
            WHERE sercpf = '{$_SESSION['cpfavaliado']}' AND tavid = ".TIPO_AVAL_CONSENSO." --AND resano = {$_SESSION['exercicio']}
        ";
        $mediaMC = $db->pegaUm( $sqlMC );

        if( $rsDados ){
            for( $i = 0; $i < count( $rsDados ); $i++ ){
                #QUANDO O USUÁRIO TIVER A GRATIFICAÇÃO GDPS - NÃO HAVERA AVALIAÇÃO PESSOAL OU EQUIPE, SENDO ASSIM, MAS QUANDO MESMO ACOMPANHAR A AVALIAÇÃO, VERÁ A AVALIAÇÃO DA CHEFIA FEITO A ELE.
                #AS DEMAIS AVALIAÇÕES, TIPO AVALIAÇÃO DE CHEFIA AO SUPORDINADO OCORRERA DA MESMA FORMA.
                #PARA QUE ISSO ACONTEÇA AS VARIAVEIS "$resposta, $avaliacao, $resposta_resid" RECEBEM VALORES.
                if( $resp['sertipogratificacao'] == 'PS' && $perm == 'N' ){
                    $resposta       = 1;
                    $avaliacao      = 'f';
                    $resposta_resid = 1;
                }
    ?>
                <tr>
                    <td> <?php echo $rsDados[$i]['comdescricao'] ?> </td>
                    <td> <?php echo $rsDados[$i]['defdescricao'] ?> </td>

                    <input type="hidden" value ="<?php echo $rsDados[$i]['defpeso'] ?>" name="pesoDefinicao[<?=$i;?>]" id ="pesoDefinicao[<?=$i;?>]">
                    <input type="hidden" value ="<?php echo $rsDados[$i]['defpesoaa'] ?>" name="pesoDefinicaoa[<?=$i;?>]" id ="pesoDefinicaoa[<?=$i;?>]">
                    <input type="hidden" value ="<?php echo $rsDados[$i]['defpesoc'] ?>" name="pesoDefinicaoc[<?=$i;?>]" id ="pesoDefinicaoc[<?=$i;?>]">

                    <?PHP

                        if( ( in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ) || ( $resp['tssid'] == 8 ) && ( $resposta_resid > 0 ) ){

                            echo getAvaliadorHTML( 'TIPO_COLUNA' , $rsDados[$i]['defpeso'], $rsDados[$i]['defpesoaa'], $rsDados[$i]['defpesoc'], $i, $rsDados[$i]['defid']);

                        }else{
                            if( !$resposta  && ($avaliacao || $avaliacao == '') ){
                                if( $tavid == 2 ){
                        ?>
                                    <td>
                                        <center>
                                           <input type="text"  size="3" maxlength="3" name="defid[<?=$rsDados[$i]['defid']?>]" onblur="<?=$mascaraGlobalJs?> validaNota(this, 'div_defid[<?=$rsDados[$i]['defid']?>]');" onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$i;?>]').value, this.value , 'div_defid[<?=$rsDados[$i]['defid']?>]', '[<?=$rsDados[$i]['defid']?>]' ); calculaColunas();" id="defid[<?=$rsDados[$i]['defid']?>]" value="">
                                        </center>
                                    </td>
                                    <td><div id="div_defid[<?=$rsDados[$i]['defid']?>]" style="display: '';"></div></td>
                        <?PHP
                                }elseif( $tavid == 1 ){
                                    if( ( $resp['tssid'] == 8 || $resp['sertipogratificacao'] == 'PS' ) ){
                                        $hab = 'disabled="disabled"';
                                    }else{
                                        $hab = '';
                                    }
                        ?>
                                    <td>
                                        <center>
                                           <input type="text"  size="3" maxlength="3" <?=$hab;?> name="defid[<?=$rsDados[$i]['defid']?>]" onblur="<?=$mascaraGlobalJs?> validaNota(this, 'div_defid[<?=$rsDados[$i]['defid']?>]');" onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicaoa[<?=$i;?>]').value, this.value , 'div_defid[<?=$rsDados[$i]['defid']?>]', '[<?=$rsDados[$i]['defid']?>]' ); calculaColunas();" id="defid[<?=$rsDados[$i]['defid']?>]" value="">
                                        </center>
                                    </td>
                                    <td><div id="div_defid[<?=$rsDados[$i]['defid']?>]"  style="display: '';"></div></td>
                        <?PHP
                                }else{
                        ?>
                                    <td>
                                        <center>
                                            <input type="text"  size="3" maxlength="3" name="defid[<?=$rsDados[$i]['defid']?>]" onblur="<?=$mascaraGlobalJs?> validaNota(this, 'div_defid[<?=$rsDados[$i]['defid']?>]');" onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicaoc[<?=$i;?>]').value, this.value , 'div_defid[<?=$rsDados[$i]['defid']?>]', '[<?=$rsDados[$i]['defid']?>]' ); calculaColunas();" id="defid[<?=$rsDados[$i]['defid']?>]" value="">
                                        </center>
                                    </td>
                                    <td><div id="div_defid[<?=$rsDados[$i]['defid']?>]"  style="display: '';"></div></td>
                        <?PHP   }
                            }else{
                                echo getAvaliadorHTML( 'TIPO_COLUNA' , $rsDados[$i]['defpeso'], $rsDados[$i]['defpesoaa'], $rsDados[$i]['defpesoc'], $i, $rsDados[$i]['defid']);
                            }
                        }
                    ?>
                </tr>
    <?PHP
            }
        }
    ?>
                <input type="hidden" name="countDefid" id="countDefid" value="<?=$i?>">
                <tr style="padding:15px; background-color:#B5B5B5; color:#404040;">
                    <td style=" font-size: 11px; font-weight: bold;">Totais</td>
                    <td>&nbsp;</td>
    <?PHP
                        if( ( in_array( PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil ) || in_array( PERFIL_SUPER_USER, $perfil ) ) || ( $resp['tssid'] == 8 ) && ( $resposta_resid > 0 ) ){
                            getAvaliadorRodapeHTML();
                        }else{
                            if( !$resposta && ($avaliacao || $avaliacao == '') ){
                                echo '<td>&nbsp;</td>';
                                echo '<td>&nbsp;</td>';
                            } else {
                                getAvaliadorRodapeHTML();
                            }
                        }
    ?>
                </tr>
        </table>

    <?PHP
        $sql = "            
            SELECT serlimpaaval
            FROM gestaopessoa.servidor
            WHERE sercpf = '{$_SESSION['cpfavaliado']}' AND seranoreferencia = {$_SESSION["exercicio"]}
        ";
        $serlimpaaval = $db->pegaUm($sql);
        
        $nota_avaliacao = calculaNotaFinal( $_SESSION['cpfavaliado'] );

        if( $serlimpaaval == 't' ){
    ?>
            <table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
                <tr>
                    <td align="center">
                        <input type="button" name="botao" value="Limpar Avaliação" onclick="limparAvaliacao('<?=$_SESSION['cpfavaliado']?>')">
                    </td>
                </tr>
            </table>
    <?  } ?>
            <table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
                <tr>
                    <td align="center"><b> Nota Final: <a id="td_nota_final"> <?=$nota_avaliacao['avaliacao_final']?> </a></b></td>
                </tr>

                <tr>
                    <td align="center"><b> Pontuação: <a id="td_pontos"> <?=$nota_avaliacao['pontos']?> </a></b></td>
                </tr>
            </table>

    <?PHP
        if( !controlaPermissao('superuser') ){
            if( $_SESSION['boautoavaliacao'] ){
                if( prazoVencido() ) {
                    $blocked = 'disabled = disabled';
                }else{
                    $blocked = ' onclick="validaForm(\'gravar\');" ';
                }
            }elseif( $_SESSION['autoavalchefe'] ){
                if( avaliacaoFinalizada( $_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR ) || prazoVencido() ) {
                    $blocked = 'disabled = disabled';
                }else{
                    $blocked = ' onclick="validaForm(\'gravar\');" ';
                }
            }else{
                if( prazoVencido() ) {
                    $blocked = 'disabled = disabled';
                }else{
                    $blocked = ' onclick="validaForm(\'gravar\');" ';
                }
            }
        }elseif( prazoVencido() ){
            $blocked = 'disabled = disabled';
        }else{
            $blocked = '';
        }

        $calculoTotal = verificaQuantidade();
    ?>

        <table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
            <tr>
                <?PHP
                    if( !$resposta && !prazoVencido() && $perm == 'S' ){
                ?>

                        <td align="center" width="8%"><input type="button" <?=$blocked?> name="bt_gravar" id="bt_gravar" value="Gravar" onclick="validaForm('gravar');"></td>

                <? }
                   if( prazoVencido() ){
                ?>
                        <td align="center" width="19%"> <input type="button" disabled="disabled" value="Gravar"> </td>
                        <td align="center" width="62%"><span style="font-size: 13px; font-weight: bold; font-style: italic;">Prazo de Avaliação Finalizado</td>

                <? } //elseif($resposta && $chefe && $avaliacao == 't' && $calculoTotal){ ?>
                    <!--
                    COMENTADO TEMPORARIAMENTE. DEFINIR REGRA DO BOTÃO FINALIZAR.
                    <td align="left"><input type="button" name="bt_finalizar" <?=$blocked?> value="Finalizar" onclick="validaForm('finalizar');"></td>
                    -->
                <? //} ?>

                    <td align="right">

                <? if( avaliacaoFinalizada( $_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO ) ){ ?>

                        <img onclick="imprimeAvaliacao();" style="cursor: pointer;" src="../imagens/print.png">
                        <span onclick="imprimeAvaliacao();" style="cursor: pointer; color: green; font-style: italic; position: relative; top: -2px;" > Imprimir Avaliação </span>

                <? } ?>
                </td>
            </tr>
        </table>
    </form>
</div>

<script type="text/javascript">

    function validaNota(v, div_id){
        var div = document.getElementById(div_id);

        if( v.value.length < 3 && v.value.length > 1){
            v.value = '0'+v.value;
        } else if( v.value.length == 1 ) {
            v.value = '00'+v.value;
        } else {
            if( v.value.length == 0 ) div.innerHTML = '';
        }
    }

    function validaForm(type){
        var action      = document.getElementById('action');
        var pendente    = document.getElementById('pendente');

        action.value = 'salvar';
        pendente.value = type;

        if( type == 'finalizar' ){
            if( podeFinalizar() ){
                if ( confirm( 'Após Finalizar a avaliação, não será possivel alterar nenhuma nota. Tem certeza que deseja finalizar?' ) ) {
                    document.formulario.submit();
                }else{
                    return false;
                }
            }else{
                alert('É necessário preencher todas as notas para finalizar esta avaliação.');
                return false;
            }
        } else if( type == 'gravar' ){
            if( validarSalvar() ){
                if (!confirm('Tem certeza que as notas informadas estão corretas?')){
                    return false;
                }
            }else{
                alert('Existem campos em branco! Preencha todos, os campos aceitão valores entre 0-100!');
                return false;
            }
        }
        document.formulario.submit();
    }


    function validarSalvar(){
        var erro = 0;

        for( var i = 1; i < 5; i++ ){
            for( var k = 31; k < 38; k++ ){
                if( document.getElementById('defid['+k+']') ){
                    if( document.getElementById('defid['+k+']').disabled == false ){
                        if( document.getElementById('defid['+k+']').value == '' ){
                            erro = 1;
                        }
                    }
                }

                if( document.getElementById('defid['+k+']['+i+']') ){
                    if( document.getElementById('defid['+i+']['+k+']').value == '' ){
                        erro = 1;
                    }
                }
            }
        }

        if(erro == 1){
            return false;
        }else{
            return true;
        }
    }

    function calcula(peso, valor, div_id, campo){
        var div = document.getElementById(div_id);
        if( Number(valor) > 100 ){
            alert('Nota não pode ser maior que 100');
            document.getElementById('defid'+campo).value=0;
            div.innerHTML = 0;
            return false;
        }
        var pesoCalculado = Number( peso * valor );
        if( !isNaN( pesoCalculado ) ){
            div.innerHTML = pesoCalculado.toFixed();
        }else{
            div.innerHTML = 0;
        }
    }

    function calculaColunas(){
        var total_aval_superior_p   = 0;
        var total_auto_aval_p       = 0;
        var total_consenso_p        = 0;
        var total_aval_superior     = 0;
        var total_auto_aval         = 0;
        var total_consenso          = 0;
        var soma1                   = 0;
        var soma2                   = 0;
        var soma3                   = 0;
        var soma1_p                 = 0;
        var soma2_p                 = 0;
        var soma3_p                 = 0;

        var soma_da_nota_final      = 0;

    	var td_nota_final   = document.getElementById('td_nota_final');
    	var td_pontos       = document.getElementById('td_pontos');

        //BLOCO PARA SOMA E DEFINIÇÃO DE NOTA.
        $('[id^="defid["]').each( function(){

            for( var k = 1; k < 4; k++ ){

                var parte = this.id.replace('defid[','');
                var i = parte.replace(']['+k+']','');

                if( document.getElementById('defid['+i+']['+k+']') ){

                    if( k == 1 ){
                        soma1 += parseFloat( document.getElementById('defid['+i+']['+k+']').value  );
                        soma1_p += Number(document.getElementById('div_defid['+i+']['+k+']').innerHTML );

                        total_auto_aval = Number( soma1   );
                        total_auto_aval_p = Number( soma1_p  );

                        if( !isNaN( total_auto_aval ) && !isNaN( total_auto_aval_p ) ){
                            if( total_auto_aval != 0 ){
                                document.getElementById('total_auto_aval').innerHTML = Math.round( total_auto_aval.toFixed() );
                            }

                            if( total_auto_aval_p != 0 ){
                                document.getElementById('total_auto_aval_p').innerHTML = Math.round( total_auto_aval_p.toFixed() );
                            }
                        }
                    }else
                        if( k == 2 ){
                            soma2 += Number(document.getElementById('defid['+i+']['+k+']').value);
                            soma2_p += Number(document.getElementById('div_defid['+i+']['+k+']').innerHTML   );

                            total_aval_superior = Number( soma2   );
                            total_aval_superior_p = Number( soma2_p   );
                            if( !isNaN( total_aval_superior ) && !isNaN( total_aval_superior_p ) ){
                                if( total_aval_superior != 0 ){
                                    document.getElementById('total_aval_superior').innerHTML = Math.round( total_aval_superior.toFixed() );
                                }

                                if( total_aval_superior_p != 0 ){
                                    document.getElementById('total_aval_superior_p').innerHTML = Math.round( total_aval_superior_p.toFixed() );
                                }
                            }
                    }else
                        if( k == 3 ){
                            soma3 += Number(document.getElementById('defid['+i+']['+k+']').value);
                            soma3_p += Number(document.getElementById('div_defid['+i+']['+k+']').innerHTML   );
                            total_consenso = Number( soma3  );
                            total_consenso_p = Number( soma3_p  );

                            if( !isNaN( total_consenso ) && !isNaN( total_consenso_p ) ){
                                if( total_consenso != 0 ){
                                    document.getElementById('total_consenso').innerHTML = Math.round( total_consenso.toFixed() );
                                }
                                if( total_consenso_p != 0 ){
                                    document.getElementById('total_consenso_p').innerHTML = Math.round( total_consenso_p.toFixed() );
                                }
                            }
                        }
                }
            }
        })
     }

    function podeFinalizar(){
        for( var i = 2; i < 10; i++ ){
            for( var k = 1; k < 4; k++ ){
                if( document.getElementById('defid['+i+']['+k+']') ){
                    if( document.getElementById('defid['+i+']['+k+']').disabled == false ){
                        if( document.getElementById('defid['+i+']['+k+']').value == '' ){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    function bloqueiaCampos(){
        for( var i = 2; i < 10; i++ ){
            for( var k = 1; k < 4; k++ ){
                if( document.getElementById('defid['+i+']['+k+']') ){
                     document.getElementById('defid['+i+']['+k+']').disabled = true;
                }
            }
        }
    }

    function limparAvaliacao( cpfavaliado ){
        if( confirm('Realmente deseja excluir toda a avaliação?') ){

            jQuery.ajax({
                url: window.location.href,
                type: "POST",
                data: '&req=limparAv&cpfavaliado='+cpfavaliado,
                onLoading: function (){
                    $('loader-container').show();
                },
                complete: function(res){
                    alert('Avaliação excluída.');
                    window.location = window.location;
                }
            });
        }
    }

    jQuery(document).ready(function(){
        if( jQuery('#ano_exercicio').val() < jQuery('#ano_atual').val() ){
            $('#formulario input').each(function(){
                if( $(this).attr('type') != 'button' && $(this).attr('type') != 'hidden' ){
                    $(this).attr('disabled', 'true');
                }
            });
            jQuery('#bt_gravar').attr('disabled', 'true');
        }
    });

    function imprimeAvaliacao(){
        window.open("gestaopessoa.php?modulo=principal/avaliacao_servidor/imp_cad_avaliacao_servidor&acao=A","Recurso Avaliação",'scrollbars=yes,height=600,width=1200,status=no,toolbar=no,menubar=no,location=no');
    }

    calculaColunas();

</script>