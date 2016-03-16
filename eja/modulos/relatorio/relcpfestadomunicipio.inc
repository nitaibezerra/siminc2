<?PHP

//$_SESSION['sislayoutbootstrap'] = 't';

    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }


    if($_POST['agrupador']){
        ini_set("memory_limit","2048M");
        $true = true;
        include("relcpfestadomunicipio_resultado.inc");
        exit;
    }

    include APPRAIZ. '/includes/Agrupador.php';
    include APPRAIZ. 'includes/cabecalho.inc';

    monta_titulo( 'Relatório Gerencial EJA Novas turmas', 'EJA Novas turmas' );
    
?>
    <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
    <html>
    <head>
    <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
    <title>Relatório</title>

    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

    <script type="text/javascript">
        function gerarRelatorio(req){
            var formulario = document.formulario;

            document.getElementById('req').value = req;

            if (formulario.elements['agrupador'][0] == null){
                alert('Selecione pelo menos um agrupador!');
                return false;
            }

            selectAllOptions( formulario.agrupador );
            selectAllOptions( formulario.colunas );
            
            var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
            janela.opener.notclose = janela;

            formulario.target = 'relatorio';
            formulario.submit();
        }

        function atualizaComboMunicipio( estuf ){
            $.ajax({
                type    : "POST",
                url     : window.location.href,
                data    : "requisicao=atualizaComboMunicipio&estuf="+estuf,
                async: false,
                success: function(resp){
                    $('#td_combo_municipio').html(resp);
                }
            });
        }
    </script>

    </head>
    <body>

    <form name="formulario" id="formulario" action="" method="post">
        <input type="hidden" name="req" id="req" value="" />

        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
            <tr>
                <td class="SubTituloDireita" width="30%">Colunas</td>
                <td>
                    <?PHP
                        $arrayColunas = array(
                            array('codigo' => 'estuf', 'descricao' => '01. Estado'),
                            array('codigo' => 'mundescricao', 'descricao' => '02. Municipio'),
                            array('codigo' => 'entnome', 'descricao' => '03. Escola'),
                            array('codigo' => 'turma_escola', 'descricao' => '04. Turma'),
                            array('codigo' => 'usunome', 'descricao' => '05. Diretor'),
                            array('codigo' => 'atecpf', 'descricao' => '06. CPF Aluno'),
                            array('codigo' => 'atenomedoaluno', 'descricao' => '07. Nome do Aluno'),
                            array('codigo' => 'ano_exercicio', 'descricao' => '08. Ano de Exercício'),					
                        );

                        $arrayColunasDest = array(
                            array('codigo' => 'atenomedoaluno', 'descricao' => '05. Nome do Aluno')					
                        ); 

                        $colunas = new Agrupador('formulario');
                        $colunas->setOrigem('naoColunas', null, $arrayColunas);
                        $colunas->setDestino('colunas', null, $arrayColunasDest);
                        $colunas->exibir();
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" valign="top">Agrupadores</td>
                <td>
                    <?PHP
                        $arrayAgrupadores = array(
                            array('codigo' => 'estuf', 'descricao' => '01. Estado'),
                            array('codigo' => 'mundescricao', 'descricao' => '02. Município'),
                            array('codigo' => 'entnome', 'descricao' => '03. Escola'),
                            array('codigo' => 'turma_escola', 'descricao' => '04. Turma')
                        );

                        $arrayAgrupadoresDest = array(
                            array('codigo' => 'mundescricao', 'descricao' => '02. Município'),
                            array('codigo' => 'entnome', 'descricao' => '03. Escola'),
                            array('codigo' => 'turma_escola', 'descricao' => '04. Turma')
                        );

                        $campoAgrupador = new Agrupador( 'formulario' );
                        $campoAgrupador->setOrigem( 'agrupadorOrigem', null, $arrayAgrupadores );
                        $campoAgrupador->setDestino( 'agrupador', null, $arrayAgrupadoresDest);
                        $campoAgrupador->exibir();
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> UF: </td>
                <td colspan="2">
                    <?PHP
                        $estuf = $dados['estuf'];
                        $sql = "
                             SELECT  estuf as codigo,
                                     estuf as descricao
                             FROM territorios.estado
                             ORDER BY estuf
                         ";
                         $db->monta_combo('estuf', $sql, 'S', "Selecione...", 'atualizaComboMunicipio', '', '', 300, 'S', 'estuf', '', $estuf, 'UF');
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> Município: </td>
                <td id="td_combo_municipio" colspan="2">
                    <?PHP
                        $muncod = $dados['muncod'];
                        $sql = "
                            SELECT  muncod AS codigo,
                                    mundescricao AS descricao
                            FROM territorios.municipio
                            ORDER BY descricao
                        ";
                        $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', 300, 'S', 'muncod', '', $muncod, 'Município');
                    ?>
                </td>
            </tr>
            
            <tr>
                <td class ="SubTituloDireita"> Turmas Abertas/ Fechadas: </td>
                <td id="td_combo_municipio" colspan="2">
                    <?PHP
                        $ntefechaturma = $dados['ntefechaturma'];
                        $sql = array(
                            array('codigo'=>'A', 'descricao'=>'Aberta'),
                            array('codigo'=>'F', 'descricao'=>'Fechada')
                        );
                        $db->monta_combo('ntefechaturma', $sql, 'S', "Selecione...", '', '', '', 300, 'S', 'ntefechaturma', '', $ntefechaturma, 'Turma Fechada');
                    ?>
                </td>
            </tr>
            
            <tr>
                <td class ="SubTituloDireita"> Turmas Validadas/ Enviada ao FNDE: </td>
                <td id="td_combo_municipio" colspan="2">
                    <?PHP
                        $nteenviofnde = $dados['nteenviofnde'];
                        $sql = array(
                            array('codigo'=>'V', 'descricao'=>'Validada'),
                            array('codigo'=>'N', 'descricao'=>'Não Validada'),
                            array('codigo'=>'E', 'descricao'=>'Enviada ao FNDE')
                        );
                        $db->monta_combo('nteenviofnde', $sql, 'S', "Selecione...", '', '', '', 300, 'S', 'nteenviofnde', '', $ntefechaturma, 'Turma Validada');
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> Ano: </td>
                <td colspan="2">
                    <?PHP
                        $ano = $dados['ano'];
                        $sql = "
                             
                                        SELECT DISTINCT ateexercicio codigo,
                                                        ateexercicio AS descricao
                                        FROM eja.alunoturmaeja
                                        WHERE ateexercicio IS NOT NULL
                                        ORDER BY codigo ASC
                         ";
                         $db->monta_combo('ano', $sql, 'S', "Selecione...", '', '', '', 300, 'S', 'estuf', '', $ano, 'ano');
                    ?>
                </td>
            </tr>
        </table>

        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
            <tr>
                <td align="center" colspan="2">
                    <input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio(1);"/>
                    <input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio(2);"/>
                </td>
            </tr>
        </table>
    </form>

    </body>
    </html>


