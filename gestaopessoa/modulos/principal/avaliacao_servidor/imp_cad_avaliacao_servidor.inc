<?PHP

    #VERIFICA SE EXISTE A SESSÃO. "USUARIO SELECIONADO", CASO NÃO VIRIFICA QUAL O PERFIL E REDIRECIONA PARA A RESPECTIVA PAGINA.
    $perfil = pegaPerfilGeral();

    if( $_SESSION['cpfavaliado'] != '' ){
        $dados = buscaDadosServidorAvaliacao( $_SESSION['cpfavaliado'] );
    }
?>
    <header>
        <style type="">
            @media print {
                .notprint {
                    display: none;
                }
                .div_rolagem{
                    display: none;
                }
                .div_rol{
                    display: none;
                }
            }

            @media screen {
                .notscreen {
                    display: none;
                }
                .div_rol{
                    display: none;
                }
            }
            .div_rolagem{
                overflow-x: auto;
                overflow-y: auto;
                height: 50px;
            }
            .div_rol{
                overflow-x: auto;
                overflow-y: auto;
                height: 50px;
            }
        </style>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

        <script type="text/javascript" src="/includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    </header>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td colspan="4" class="subTituloCentro" style="font-size: 12px; background-color: #dcdcdc;">
                Formulário de Avaliação de Desempenho Individual - <?= $dados['sergratificacao'] ? $dados['sergratificacao'] : 'TIPO DE GRATIFICAÇÃO NÃO INFORMADO';?> <br> Ciclo Avaliativo: <?=DATA_CICLO_AVALIATIVO_INICIAL;?> a <?=DATA_CICLO_AVALIATIVO_FINAL;?>
            </td>
        </tr>
        <!--
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
        -->
        <tr>
            <td class="subTituloDireita" width="30%">Servidor:</td>
            <td>
                <?php
                    $sernome = $dados['sernome'];
                    echo campo_texto('sernome', 'N', 'N', '', 50, 14, '', '', '', '', 0, 'id="sernome"', '', $sernome, '', null);
                ?>
            </td>
            <td class="subTituloDireita">SIAPE:</td>
            <td>
                <?php
                    $sersiape = $dados['sersiape'];
                    echo campo_texto('sersiape', 'N', 'N', '', 50, 14, '', '', '', '', 0, 'id="sersiape"', '', $sersiape, '', null);
                ?>
            </td>
        </tr>
        <!--
        <tr>
        </tr-->
        <tr>
            <td class="subTituloDireita">Cargo Efetivo:</td>
            <td>
                <?php
                    $sercargo = $dados['sercargo'];
                    echo campo_texto('sercargo', 'N', 'N', '', 50, 14, '', '', '', '', 0, 'id="sercargo"', '', $sercargo, '', null);
                ?>
            </td>
            <td class="subTituloDireita">Chefe:</td>
            <td>
                <?php
                    $serdescricao_chefe = $dados['serdescricao_chefe'];
                    echo campo_texto('serdescricao_chefe', 'N', 'N', '', 50, 14, '', '', '', '', 0, 'id="serdescricao_chefe"', '', $serdescricao_chefe, '', null);
                ?>
            </td>
        </tr>
        <<!--
        <tr>
        </tr>
         -->
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
                <td  colspan="4" class="subtituloEsquerda" width="15%" style="font-size: 13px; background-color: #e9e9e9;"> Fatores de Avaliação </td>
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
                } else {
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
                                                <input type="text"  size="3" readonly="readonly" maxlength="3" name="defid[<?=$rsDados[$i]['defid']?>]" onblur="<?=$mascaraGlobalJs?> validaNota(this, 'div_defid[<?=$rsDados[$i]['defid']?>]');" onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$i;?>]').value, this.value , 'div_defid[<?=$rsDados[$i]['defid']?>]', '[<?=$rsDados[$i]['defid']?>]' ); calculaColunas();" id="defid[<?=$rsDados[$i]['defid']?>]" value="">
                                            </center>
                                        </td>
                                        <td><div id="div_defid[<?=$rsDados[$i]['defid']?>]" style="display: '';"></div></td>
                        <?PHP
                                    } elseif( $tavid == 1 ){ 
                                        if( ( $resp['tssid'] == 8 || $resp['sertipogratificacao'] == 'PS' ) ){
                                            $hab = 'disabled="disabled"';
                                        }else{
                                            $hab = '';
                                        }
                        ?>
                                        <td>
                                            <center>
                                                <input type="text"  size="3" readonly="readonly" maxlength="3" <?=$hab;?> name="defid[<?=$rsDados[$i]['defid']?>]" onblur="<?=$mascaraGlobalJs?> validaNota(this, 'div_defid[<?=$rsDados[$i]['defid']?>]');" onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicaoa[<?=$i;?>]').value, this.value , 'div_defid[<?=$rsDados[$i]['defid']?>]', '[<?=$rsDados[$i]['defid']?>]' ); calculaColunas();" id="defid[<?=$rsDados[$i]['defid']?>]" value="">
                                            </center>
                                        </td>
                                        <td><div id="div_defid[<?=$rsDados[$i]['defid']?>]"  style="display: '';"></div></td>
                        <?PHP
                                    } else {
                        ?>
                                        <td>
                                            <center>
                                                <input type="text"  size="3" readonly="readonly" maxlength="3" name="defid[<?=$rsDados[$i]['defid']?>]" onblur="<?=$mascaraGlobalJs?> validaNota(this, 'div_defid[<?=$rsDados[$i]['defid']?>]');" onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicaoc[<?=$i;?>]').value, this.value , 'div_defid[<?=$rsDados[$i]['defid']?>]', '[<?=$rsDados[$i]['defid']?>]' ); calculaColunas();" id="defid[<?=$rsDados[$i]['defid']?>]" value="">
                                            </center>
                                        </td>
                                        <td><div id="div_defid[<?=$rsDados[$i]['defid']?>]"  style="display: '';"></div></td>
                        <?PHP       }
                                } else {
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
            $nota_avaliacao = calculaNotaFinal($_SESSION['cpfavaliado']);
        ?>

        <table style="margin-top: 3px; background-color:#e9e9e9; color:#404040; height:10px;" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td align="right"><b> Nota Final: <a id="td_nota_final"> <?=$nota_avaliacao['avaliacao_final']?> </a></b></td>
                <td align="left"><b> Pontuação: <a id="td_pontos"> <?=$nota_avaliacao['pontos']?> </a></b></td>
            </tr>
        </table>

        <table style="margin-top: 3px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr style="text-align:center; height:35px; vertical-align: bottom;">
                <td> Local e Data: ______________, ___ de __________ de <?=$_SESSION['exercicio'];?>  </td>
                <td> Local e Data: ______________, ___ de __________ de <?=$_SESSION['exercicio'];?>  </td>
            </tr>
            <tr style="text-align:center; height:60px; vertical-align: bottom;">
                <td align="center">
                    Assinatura <br> <b> Avaliado(Servidor) </b>
                </td>
                <td align="center">
                    Assinatura <br> <b> Avaliador(Chefia) </b>
                </td>
            </tr>
        </table>
            <br>
        <div id = "div_rolagem" class="notprint">
            <table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td colspan="2" class="subtituloCentro">
                        <input type="button" name="bt_imprimir" value="Imprimir" onclick="javascript: window.print();">
                        <input type="button" name="bt_fechar" value="Fechar" onclick="javascript: window.close();">
                    </td>
                </tr>
            </table>
        </div>

    </form>

<script type="text/javascript">


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
     }

    calculaColunas();

</script>