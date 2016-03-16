<?php
$fdpcpf = $_SESSION['cpfavaliado'];

if( $fdpcpf ){
        $sql = "
            SELECT  ftprid,
                    fdpcpf,
                    ftprboletimserv, ftprdtboletimserv,
                    ftprconmettecdesatcaref, ftprpontrecebida, ftprpontsolicitada,
                    ftprprodtrabalho, ftprpontrecebidaprodtrab, ftprpontsolicprodtrab,
                    ftprcapautdesen, ftprpontrecebcapdesen, ftprpontsoliccapdesen,
                    ftprrelacinterpessoal, ftprpontrecebrelintpes, ftprpontsolicrelintpes,
                    ftprtrabequipe, ftprpontrecebtrabequipe, ftprpontsolictrabequipe,
                    ftprcomprtrabalho, ftprpontrecebcomptrab, ftprpontsoliccomptrab,
                    ftprcumpnorproccodatrcarg, ftprpontrecebcumpnorproccodatrcarg, ftprpontsoliccumpnorproccodatrcarg,
                    flprparcerchefia,
                    ftprpedido,
                    ftprcienavaliador,
                    ftprliberaparecerchefe

            FROM gestaopessoa.ftpedidoreconsideracao

            WHERE fdpcpf = '$fdpcpf' AND anoreferencia = {$_SESSION['exercicio']}
        ";
        $arrDados = $db->pegaLinha( $sql );
        $arrDados = $arrDados ? $arrDados : array();
        extract($arrDados);

        $sql = "
            SELECT  ser.sercpf,
                    ser.sernome,
                    ser.sersiape,
                    ser.sercargo,
                    ser.sercodigofuncao,
                    tls.tlssigla,
                    ser.sercpfchefe,
                    --BASE SEM PADRÃO, FORMATAÇÃO.
                    --TRIM( TO_CHAR( CAST(REPLACE( fun.fdftelefone, '-', '' ) as int), '0000-0000') ) AS fdftelefone
                    fdftelefone
            FROM gestaopessoa.servidor AS ser
            LEFT JOIN gestaopessoa.ftdadofuncional AS fun ON fun.fdpcpf = ser.sercpf
            LEFT JOIN gestaopessoa.tipolotacaoservidor AS tls on tls.tlsid = ser.tlsid and tls.tlsstatus = 'A'

            WHERE ser.sercpf = '$fdpcpf' AND ser.seranoreferencia = {$_SESSION['exercicio']}
        ";
        $arrServidor = $db->pegaLinha( $sql );

        $sql = "
            SELECT  DISTINCT ser.sercpf,
                    ser.sernome,
                    ser.sersiape,
                    ser.sercargo,
                    tls.tlssigla,
                    ser.sercodigofuncao,
                    TRIM( TO_CHAR( CAST(REPLACE( fun.fdftelefone, '-', '' ) as int), '0000-0000') ) AS fdftelefone
            FROM gestaopessoa.servidor ser
            LEFT JOIN gestaopessoa.ftdadofuncional AS fun ON fun.fdpcpf = ser.sercpf
            LEFT JOIN gestaopessoa.tipolotacaoservidor AS tls ON tls.tlsid = ser.tlsid AND tls.tlsstatus = 'A'

            WHERE ser.sercpf = '{$arrServidor['sercpfchefe']}' AND ser.seranoreferencia = {$_SESSION['exercicio']}
        ";
        $arrChefe = $db->pegaLinha( $sql );
    }

    $imagem = "<img style=\"cursor:pointer\" src=\"/imagens/check.jpg\" border=\"0\">";

    #VERIFICA SE O USUÁRIO É O CHEFE OU O PROPRIO. SE FECHE ELE IMPRIME O FORMULÁRIO COMPLETO (COM SO PARECERES: "IV ? Reconsideração da Chefia Imediata ou do Substituto", "V ? Responsabilidade da Coordenação-Geral de Gestão de Pessoas")
    $tipo_usu = buscaPermissaoReconcideracao( $fdpcpf );
    
    #BUSCA O TIPO DE GRATIFICAÇÃO QUE É ATRIBUIDA AO SERVIDOR AVALIADO.
    $tp_gra = verificaTipoGratificacaoServidor( $_SESSION['cpfavaliado'] );
?>

<html>
	<head>
            <meta http-equiv="Cache-Control" content="no-cache">
            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	</head>
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
	<body>
    <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
                <td colspan="5" class="subTituloCentro" style="font-size: 15px; background-color: #dcdcdc;">
                    Formulário de Pedido de Reconsideração - Ciclo Avaliativo:  <?=DATA_PERIODO_INICO_AVALIACAO;?> a <?=DATA_PERIODO_FINAL_AVALIACAO;?>
                </td>
            </tr>
            <tr>
                <td colspan="5" class="subTituloCentro" style="font-size: 10px; background-color: #dcdcdc;">
                    Gratificação de Desempenho do Plano Geral de Cargos do Poder Executivo - Decreto nº 7.133, de 19 de março de 2010
                </td>
            </tr>

            <tr>
                <td colspan="2" class="subtituloEsquerda" width="45%" style="font-size: 11px; background-color: #e9e9e9;"> Identificação do(a) Servidor(a) </td>
                <td width="1%" style="background-color: #B5B5B5;">&nbsp;</td>
                <td colspan="2" class="subtituloEsquerda" width="54%" style="font-size: 11px; background-color: #e9e9e9;"> Identificação do(a) Avaliador(a) </td>
            </tr>

            <tr>
                <!-- DADOS SERVIDOR -->
                <td class="subTituloDireita"> Nome: </td>
                <td> <?=$arrServidor['sernome'];?> </td>
                <td width="1%" style="background-color: #B5B5B5;">&nbsp;</td>

                <!-- DADOS CHEFE AVALIADOR -->
                <td class="subTituloDireita"> Nome: </td>
                <td> <?=$arrChefe['sernome'];?> </td>
            </tr>
            <tr>
                <!-- DADOS SERVIDOR -->
                <td class="subTituloDireita"> SIAPE: </td>
                <td> <?=$arrServidor['sersiape'];?> </td>
                <td style="background-color: #B5B5B5;">&nbsp;</td>

                <!-- DADOS CHEFE AVALIADOR -->
                <td class="subTituloDireita"> SIAPE: </td>
                <td> <?=$arrChefe['sersiape'];?> </td>
            </tr>
            <tr>
                <!-- DADOS SERVIDOR -->
                <td class="subTituloDireita"> Cargo: </td>
                <td> <?=$arrServidor['sercargo'];?> </td>
                <td style="background-color: #B5B5B5;">&nbsp;</td>

                <!-- DADOS CHEFE AVALIADOR -->
                <td class="subTituloDireita"> Cargo: </td>
                <td> <?=$arrChefe['sercargo'];?> </td>
            </tr>
            <tr>
                <!-- DADOS SERVIDOR -->
                <td class="subTituloDireita"> Função: </td>
                <td> <?=$arrServidor['sercodigofuncao'];?> </td>
                <td style="background-color: #B5B5B5;">&nbsp;</td>

                <!-- DADOS CHEFE AVALIADOR -->
                <td class="subTituloDireita"> Função: </td>
                <td> <?=$arrChefe['sercodigofuncao'];?> </td>
            </tr>
            <tr>
                <!-- DADOS SERVIDOR -->
                <td class="subTituloDireita"> Unidade de Exercicio: </td>
                <td> <?=$arrServidor['tlssigla'];?> </td>
                <td style="background-color: #B5B5B5;">&nbsp;</td>

                <!-- DADOS CHEFE AVALIADOR -->
                <td class="subTituloDireita"> Unidade de Exercicio: </td>
                <td> <?=$arrChefe['tlssigla'];?> </td>
            </tr>
            <tr>
                <!-- DADOS SERVIDOR -->
                <td class="subTituloDireita"> Telefone (Ramal): </td>
                <td> <?=$arrServidor['fdftelefone'];?> </td>
                <td style="background-color: #B5B5B5;">&nbsp;</td>

                <!-- DADOS CHEFE AVALIADOR -->
                <td class="subTituloDireita"> Telefone (Ramal): </td>
                <td> <?=$arrChefe['fdftelefone'];?> </td>
            </tr>
        </table>
        <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td colspan="4" class="subtituloEsquerda" width="15%" style="font-size: 11px; background-color: #e9e9e9;"> Manifestação Expressa do(a) Servidor(a) </td>
            </tr>
            <tr>
                <td colspan="4" class="textoJustificado">
                    <p>
                        Por intermédio deste documento venho requerer ao meu Chefe Imediato ou ao seu Substituto, responsável pela minha Avaliação de Desempenho Individual referente à
                        Gratificação de Desempenho do Plano Geral de Cargos do Poder Executivo - GDPGPE, RECONSIDERAÇÃO do resultado final obtido na Avaliação Individual,
                        com base nas seguintes justificativas:
                        <br>
                        <!--
                        <span style="font-size: 10px">(Descrever o motivo da não concordância com a avaliação de desempenho, apontando por Fator de Avaliação)</span>
                        -->
                    </p>
                </td>
            </tr>
        </table>
        <br>
        <table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td class="subtituloEsquerda" colspan="2">Conhecimento de métodos e técnicas para o desenvolvimento das atividades do cargo efetivo</td>
            </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td valign="top" width="90%">Justificativa:<br>
                        <? echo $ftprconmettecdesatcaref; ?></td>
                    <td valign="top">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=($ftprpontrecebida ? $ftprpontrecebida : '0') ?> </td>
                                <td><?=($ftprpontsolicitada ? $ftprpontsolicitada : '0') ?> </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <?php
            if( $tp_gra != 'PS'){
        ?>        
        <tr>
            <td class="subtituloEsquerda" colspan="2">Produtividade no trabalho</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td valign="top" width="90%">Justificativa:<br>
                        <? echo $ftprprodtrabalho; ?></td>
                    <td valign="top" width="10%">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=($ftprpontrecebidaprodtrab ? $ftprpontrecebidaprodtrab : '0') ?> </td>
                                <td><?=($ftprpontsolicprodtrab ? $ftprpontsolicprodtrab : '0') ?> </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda" colspan="2">Capacidade de auto desenvolvimento</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td valign="top" width="90%">Justificativa:<br>
                        <? echo $ftprcapautdesen; ?></td>
                    <td valign="top" width="10%">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=($ftprpontrecebcapdesen ? $ftprpontrecebcapdesen : '0') ?> </td>
                                <td><?=($ftprpontsoliccapdesen ? $ftprpontsoliccapdesen : '0') ?> </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda" colspan="2">Relacionamento interpessoal</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td valign="top" width="90%">Justificativa:<br>
                        <? echo $ftprrelacinterpessoal; ?></td>
                    <td valign="top" width="10%">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=($ftprpontrecebrelintpes ? $ftprpontrecebrelintpes : '0') ?> </td>
                                <td><?=($ftprpontsolicrelintpes ? $ftprpontsolicrelintpes : '0') ?> </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <?php
            }
        ?>
        <tr>
            <td class="subtituloEsquerda" colspan="2">Trabalho em equipe</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td valign="top" width="90%">Justificativa:<br>
                        <? echo $ftprtrabequipe; ?></td>
                    <td valign="top" width="10%">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=($ftprpontrecebtrabequipe ? $ftprpontrecebtrabequipe : '0') ?> </td>
                                <td><?=($ftprpontsolictrabequipe ? $ftprpontsolictrabequipe : '0') ?> </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda" colspan="2">Comprometimento com trabalho</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td valign="top" width="90%">Justificativa:<br>
                        <? echo $ftprcomprtrabalho; ?></td>
                    <td valign="top" width="10%">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=($ftprpontrecebcomptrab ? $ftprpontrecebcomptrab : '0') ?> </td>
                                <td><?=($ftprpontsoliccomptrab ? $ftprpontsoliccomptrab : '0') ?> </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda" colspan="2">Cumprimento das normas de procedimento de conduta no desempenho das atribuições do cargo</td>
        </tr>
        <tr>
            <td colspan="2">
            <table align="left" style="width: 100%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td valign="top" width="90%">Justificativa:<br>
                        <? echo $ftprcumpnorproccodatrcarg; ?></td>
                    <td valign="top" width="10%">
                        <table align="left" style="width: 10%" border="0" class="tabela" cellpadding="3" cellspacing="1">
                            <tr>
                                <td class="subtitulocentro" colspan="2">Pontuação</td>
                            </tr>
                            <tr>
                                <td class="subtitulocentro">Recebida</td>
                                <td class="subtitulocentro">Solicitada</td>
                            </tr>
                            <tr style="text-align: center;">
                                <td><?=($ftprpontrecebcumpnorproccodatrcarg ? $ftprpontrecebcumpnorproccodatrcarg : '0') ?> </td>
                                <td><?=($ftprpontsoliccumpnorproccodatrcarg ? $ftprpontsoliccumpnorproccodatrcarg : '0') ?> </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
            </td>
        </tr>
        <tr>
            <td colspan="2">Nestes termos, pede deferimento.</td>
        </tr>
        <tr>
            <td style="text-align: center; height: 100px" colspan="2">
            Local e Data: _______________, ____ de ____________ de <?=date('Y'); ?>.</td>
        </tr>
        <tr>
            <td style="text-align: center;" colspan="2">___________________________________<br> Assinatura do(a) Servidor(a) Avaliado(a)
            </td>
        </tr>
        <?PHP

        if( $tipo_usu['usuario_tipo'] == 'CH'){

        ?>
            <tr>
                <td class="subtituloEsquerda" colspan="2">IV – Reconsideração da Chefia Imediata ou do Substituto</td>
            </tr>
            <tr>
                <td colspan="2" valign="top">Parecer:<br>
                <? echo $flprparcerchefia; ?></td>
            </tr>
            <tr>
                <td colspan="2"><b>Conclusão:</b></td>
            </tr>
            <tr>
                <td colspan="2">
                    <table align="left" style="width: 100%" border="0" cellpadding="3" cellspacing="1">
                    <tr>
                        <td width="33.3%" valign="top">(<?=(($ftprpedido == 'D') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Pedido Deferido </td>
                        <td width="33.3%" valign="top">(<?=(($ftprpedido == 'P') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Pedido Deferido Parcialmente </td>
                        <td width="33.3%" valign="top">(<?=(($ftprpedido == 'I') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Pedido Indeferido </td>
                    </tr>
                    </table>
                    </td>
            </tr>
            <tr>
                <td style="text-align: center; height: 100px" colspan="2">Local e Data: _______________, ____ de ____________ de <?=date('Y'); ?>.</td>
            </tr>
            <tr>
                <td style="text-align: center;" colspan="2">_____________________________________________<br> Assinatura e carimbo da Chefia Imediata ou Substituto</td>
            </tr>
            <tr>
                <td class="subtituloEsquerda" colspan="2">V – Responsabilidade da Coordenação-Geral de Gestão de Pessoas</td>
            </tr>
            <tr>
                <td colspan="2">Ciência do(a) Avaliado(a) do resultado do Pedido de Reconsideração:</td>
            </tr>
            <tr>
                <td colspan="2">
                <table align="left" style="width: 100%" border="0" cellpadding="3" cellspacing="1">
                    <tr>
                        <td width="33.3%" valign="top">(<?=(($ftprcienavaliador == 'CO') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Concordo </td>
                        <td width="33.3%" valign="top">(<?=(($ftprcienavaliador == 'DR') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Discordo, informando que irei interpor Recurso. </td>
                        <td width="33.3%" valign="top">(<?=(($ftprcienavaliador == 'DN') ? $imagem : '&nbsp;&nbsp;&nbsp;&nbsp;') ?>) Discordo, porém não vou interpor Recurso. </td>
                    </tr>
                </table></td>
            </tr>
            <tr>
                <td style="text-align: center; height: 100px" colspan="2">Local e Data: _______________, ____ de ____________ de <?=date('Y'); ?>.</td>
            </tr>
            <tr>
                <td style="text-align: center;" colspan="2">_____________________________________________<br> Assinatura do(a) Servidor(a) Avaliado(a)<br><br></td>
            </tr>
        <?PHP
        }
        ?>
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

</body>
</html>