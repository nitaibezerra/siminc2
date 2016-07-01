<?PHP

    include_once APPRAIZ . "academico/modulos/principal/edu_profissional/autorizacaoInauguracaoController.php";
    
    $ingid = $_REQUEST['ingid'];

    if (!empty($ingid)) {
        $dados = buscaDadosInauguracao($ingid);
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
            
            .textoJustificado{
                /*font-weight: bold;*/
                font-size: 12px;
                color: black;
                font-family: arial, verdana;
                background-color: #f0f0f0;
                text-align: justify
            }
        </style>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    </header>

    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela notprint" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro"> IMPRESSÃO FORMULÁRIO DE AUTORIZAÇÃO DE INAUGURAÇÃO </td>
        </tr>
    </table>
    <br>
    <table align="center" bgcolor="#f5f5f5" border="1" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro" style="text-align: left;" colspan="4"> Dados da Inauguração</td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width:30%;">Inauguração:</td>
            <td colspan="3">
                <input type="radio" name="ingtipo" id="ingtipo_r" disabled="disabled" <?=($dados['ingtipo'] == 'R' ? 'checked="checked"' : '');?> > Reitoria
                <input type="radio" name="ingtipo" id="ingtipo_u" disabled="disabled" <?=($dados['ingtipo'] == 'U' ? 'checked="checked"' : '');?> > Unidade
                <input type="radio" name="ingtipo" id="ingtipo_n" disabled="disabled" <?=($dados['ingtipo'] == 'O' ? 'checked="checked"' : '');?> > Nova Obra
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Instituição:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    $entidies_dsc = $db->pegaUm("SELECT entnome FROM entidade.entidade WHERE entid = {$_SESSION['academico']['entid']}");
                    echo $entidies_dsc;
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Unidade:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    if( trim($dados['entidcampus']) != '' ){
                        $entidcampus = $db->pegaUm("SELECT entnome FROM entidade.entidade WHERE entid = {$dados['entidcampus']}");
                        echo $entidcampus;                        
                    }else{
                        echo ' - ';
                    }
                ?>
            </td>
        </tr>
         <tr>
            <td class="SubTituloDireita">Obra:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['ingdscobra'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Reitor:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['ingdscreitor'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Dirigente da Unidade:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['ingnomedirigente'];
                ?>
            </td>
        </tr>

        <tr>
            <td class="subTituloCentro" colspan="4" style="text-align: left;"> Dada de inicio de Funcionamento:</td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Instalações Porvisórias:</td>
            <td class="textoJustificado">
                <?PHP
                    echo $dados['ingdtinstprov'];
                ?>
            </td>
            <td class="SubTituloDireita"> Instalações Definitivas:</td>
            <td class="textoJustificado">
                <?PHP
                    echo $dados['ingdtinstdef'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Sugestão de data da inauguração: </td>
            <td colspan="3">
                <?PHP
                    echo $dados['ingsuginauguracao'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Justificativa: </td>
            <td colspan="3">
                <?PHP
                    echo $dados['inginaugjustificativa'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="subTituloCentro" colspan="4" style="text-align: left;"> Endereço</td>
        </tr>

        <tr>
            <td class="SubTituloDireita"> CEP:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    $edocep = substr($dados['edocep'], 0, 5)."-".substr($dados['edocep'], 5, 3);
                    echo $edocep;
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Logradouro:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['edodsc'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Complemento:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['edocomplemento'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Bairro:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['edobairro'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> UF:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['estuf'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Município:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    $muncod = $db->pegaUm("SELECT  mundescricao FROM territorios.municipio WHERE muncod = '{$dados['muncod']}'");
                    echo $muncod; 
                ?>
            </td>
        </tr>
        <tr>
            <td class="subTituloCentro" colspan="4" style="text-align: left;"> Cursos</td>
        </tr>

        <tr>
            <td class="SubTituloDireita"> Listagem de Cursos:</td>
            <td class="SubTituloDireita" colspan="3">
                <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <td>
                            <?PHP
                                if( $ingid != '' ){
                                    $sql = "
                                        SELECT  crsdsc, 
                                                crsmatricula, 
                                                tpcdsc, 
                                                CASE 
                                                    WHEN crsmodalidade = 'R' THEN 'Modalidade - Regular'
                                                    WHEN crsmodalidade = 'B' THEN 'Modalidade - BF'
                                                    WHEN crsmodalidade = 'E' THEN 'Modalidade - e-Tec'
                                                END AS crsmodalidade                                                
                                        FROM academico.curso AS c
                                        JOIN academico.tipocurso AS t ON t.tpcid = c.tpcid
                                        WHERE ingid = {$ingid}
                                    ";
                                    $cursoCadastrados = $db->carregar($sql);

                                    $cabecalho = array("Curso", "Número de matrículas", "Tipo", "Regular ou Bf ou e-Tec");
                                    $alinhamento = Array('', 'center', 'center', '');
                                    $tamanho = Array('30%', '10%', '20%', '', '');
                                    $param['totalLinhas'] = false;
                                    if(is_array($cursoCadastrados)){
                                        $db->monta_lista($cursoCadastrados, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento, '', $param);
                                    }else{
                                        echo <<<HTML
                                            <table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tbody><tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr></tbody></table>
HTML;
                                    }
                                }
                            ?>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>

        <tr>
            <td class="subTituloCentro" colspan="4" style="text-align: left;"> Pessoal</td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Docentes previstos:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['ingnumdocprevisto'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Docentes nomeados:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['ingnumdocnomeados'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Téc. Adm. Previstos:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['ingnumtecprevisto'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Téc. Adm. Nomeados:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['ingnumtecnomeados'];
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Investimento (custo da obra):</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['ingvlrinvestimento'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Investimento Mobilario e Equipamentos:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['ingvlrinvmobequip'];
                ?>
            </td>
        </tr>

        <tr>
            <td class="subTituloCentro" colspan="4" style="text-align: left;"> Relatório Fotográfico do Conjunto da obra</td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Listagem de Fotos:</td>
            <td class="SubTituloDireita" colspan="3">
                <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <?php
                        $fotos = carregarFotosObrasInaug($ingid);
                        if(is_array($fotos)){
                            
                            foreach ($fotos as $key => $foto) {
                                $arrIdsFotos[] = filter_var($foto['arqdescricao'], FILTER_SANITIZE_NUMBER_INT);
                            }

                            for ($i = 1; $i <= 6; $i++){
                                if($i == 1 || $i == 5) echo "<tr>";
                                $key = array_search($i, $arrIdsFotos);
                                    
                                if ($key !== false){
                                    $key++
                    ?>
                                    <td style='width: 25%;'>
                                        <div id="div_foto<?= $i; ?>" class="imgAbrirGaleria" style='width:100%;height:120px;'>
                                            <a id="fancybox_img_<?=$fotos[$key]['arqid'];?>" class="fancybox_img fancybox.ajax" data-fancybox-group="gallery" href="academico.php?modulo=principal/edu_profissional/autorizacao_inauguracao&acao=A&exibirfoto=1&arqid=<?=$fotos[$key]['arqid'];?>">
                                                <img class="img-thumbnail" style="height:100%; width:98% !important;" src="/slideshow/slideshow/verimagem.php?&arqid=<?=$fotos[$key]['arqid'];?>"/>
                                            </a>
                                        </div>
                                    </td>
                    <?php
                                }else{
                    ?>
                                    <td><div id="div_foto<?= $i; ?>" class="imgAbrirGaleria"></div></td>
                    <?php
                                }
                                if($i == 4) echo "</tr>";
                            }
                            
                        }else{
                    ?>
                            <tr><td><table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tbody><tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr></tbody></table></td></tr>
                    <?php
                        }
                    ?>
                </table>
            </td>
        </tr>

        <tr>
            <td class="subTituloCentro" colspan="4" style="text-align: left;"> Breve Histórico da Unidade</td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Descrição:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['inghistoricound'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="subTituloCentro" colspan="4" style="text-align: left;"> Informações do Município</td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Descrição:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['inginfgeral'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="subTituloCentro" colspan="4" style="text-align: left;"> Informações gerais sobre as condições da obra</td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Descrição:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['inginfcondobra'];
                ?>
            </td>
        </tr>
        <tr>
            <td class="subTituloCentro" colspan="4" style="text-align: left;"> Observações:</td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Descrição:</td>
            <td colspan="3" class="textoJustificado">
                <?PHP
                    echo $dados['ingiobservacoes'];
                ?>
            </td>
        </tr>
    </table>
    <br>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela notprint" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro" colspan="4">
                <input type="button" id="salvar" name="salvar" value="Imprimir Formulário" onclick="window.print();"/>
                <input type="button" id="voltar" name="voltar" value="Cancelar" onclick="window.close();"/>
            </td>
        </tr>
    </table>