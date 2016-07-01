<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }
?>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    
    <style type="text/css">
        .cabecalho{
            text-align:center;
            font-weight:bold;
            border-left: 1px solid rgb(255, 255, 255); 
            border-right: 1px solid rgb(192, 192, 192); 
            border-bottom: 1px solid rgb(192, 192, 192);
        }
    </style>

    <table bgcolor= "#f5f5f5 " cellSpacing= "1 " cellPadding= "3 " align= "center" class="tabela">
        <tr>
            <td class="subTituloCentro"> Carga SIAPE por meio Upload de arquivos CSV </td>
        </tr>
    </table>

    <table bgcolor= "#f5f5f5" cellSpacing= "1" cellPadding= "3" align= "center" class="tabela">        
        <tr>
            <td colspan="6">
                <b>1. DESCRIÇÃO</b>

                <span style= "text-align:justify " >
                    <p>
                        A carga SIAPE foi criado para viabilizar um meio mais rápido para a inclução de novos professores no sistema RSC, por meio de transferência de arquivos.
                        Esta opção permite que você faça o download de um arquivo padronizado, onde os dados dos professores possam ser inseridas por meio do upload de arquivo.
                    </p>
                    <p>
                        A transferência de dados entre o usuário e o sistema RSC deve ser feita através de um arquivo em formato CSV (ver exemplo abaixo).
                        O conteúdo dos arquivos deverá obedecer a estrutura definida, ou seja, a estrutura dos dados não deverá mudar.
                        Em seguida, será possível realizar o processo de upload para que as dados contidos no arquivo, sejam devidamente iseridas no sistema.
                    </p>
                    <p style= "color:#0000CD " >
                        <b>Dica:</b> 
                        Caso o seu órgão tenha uma equipe de suporte à Tecnologia da Informação - TI, entre em contato com eles para obter ajuda na geração do arquivo em formato CSV.
                    </p>
                </span>

                <b>2. EXEMPLO DO ARQUIVO CSV</b>
                <p>
                    Com objetivo de facilitar o entendimento da estrutura do arquivo CSV, exemplificamos abaixo uma estrutura simples do arquivo esperado pelo
                    sistema <span style= "color:#0000CD " > RSC </span> para atualização do cadastro de servidores. Cada coluna do arquivo está separada por cores
                    que representam as seguintes informações:
                </p>
            </td>
        </tr>
        <tr>
            <td>
                <span style="font-weight: bold;"> - Matrícula SIAPE: 08 caracteres. </span> <br>
                <span style="font-weight: bold;"> - Nome do servidor: 100 caracteres. </span> <br>
                <span style="font-weight: bold;"> - Código do Orgão: 7 caracteres. </span> <br>
                <span style="font-weight: bold;"> - CPF: 11 caracteres. </span> <br>
                <span style="font-weight: bold;"> - Data de nascimento: 99/99/9999 </span> <br>
                <span style="font-weight: bold;"> - Tipo: 02 caracteres (AP-Aposentado / AT-Atuante). </span>
            </td>
        </tr>
        <tr>
            <td colspan="6"> <b> 2.1 COLUNAS - INFORMATIVO </b> </td>
        </tr>
        <tr>
            <td>
                <span style="color: #FF0000;"> Matrícula SIAPE </span>;
                <span style="color: #B22222;"> Nome do servidor </span>;
                <span style="color: #0059b3;"> Código do Orgão </span>;
                <span style="color: #228B22;"> CPF </span>;
                <span style="color: #006400;"> Data de nascimento </span>;
                <span style="color: #483D8B;"> Tipo </span>; <br>

                <span style="color: #FF0000;"> x</span>; <span style="color: #B22222;"> x</span>; <span style="color: #0059b3;"> x</span>; <span style="color: #228B22;"> x</span>; <span style="color: #006400;"> x</span>; <span style="color: #483D8B;"> x</span> <br>
                <span style="color: #FF0000;"> x</span>; <span style="color: #B22222;"> x</span>; <span style="color: #0059b3;"> x</span>; <span style="color: #228B22;"> x</span>; <span style="color: #006400;"> x</span>; <span style="color: #483D8B;"> x</span> <br>
                <span style="color: #FF0000;"> x</span>; <span style="color: #B22222;"> x</span>; <span style="color: #0059b3;"> x</span>; <span style="color: #228B22;"> x</span>; <span style="color: #006400;"> x</span>; <span style="color: #483D8B;"> x</span> <br>
                <span style="color: #FF0000;"> x</span>; <span style="color: #B22222;"> x</span>; <span style="color: #0059b3;"> x</span>; <span style="color: #228B22;"> x</span>; <span style="color: #006400;"> x</span>; <span style="color: #483D8B;"> x</span> <br>
                <span style="color: #FF0000;"> x</span>; <span style="color: #B22222;"> x</span>; <span style="color: #0059b3;"> x</span>; <span style="color: #228B22;"> x</span>; <span style="color: #006400;"> x</span>; <span style="color: #483D8B;"> x</span> <br>
                <span style="color: #FF0000;"> x</span>; <span style="color: #B22222;"> x</span>; <span style="color: #0059b3;"> x</span>; <span style="color: #228B22;"> x</span>; <span style="color: #006400;"> x</span>; <span style="color: #483D8B;"> x</span> <br><br>
            </td>
        </tr>
        <tr>
            <td>
                <b> Atenção: </b><br>
                1) Não é nescessário conter o título (cabeçalho) dos campos.</br>
                2) Todos os campos deverão ser separados por ponto e vírgula ";".</br>
                3) Os valores deverão ser escritos sem espaços, sem vigulas, sem caracteres especiais etc. Apenas no fomato mostrado acima.<br>
                4) O nome do arquivo não pode conter ponto(s) <b style="color: red;">"."</b>. Por exemplo: <b><i>servidor.carga.mes.novembro.csv</i></b>, caso isso ocorra mude para, <b><i>servidor_carga_mes_novembro.csv.</i></b><br>
                5) Para visualizar um exemplo do arquivo CSV especificamente para este indicador, <a href='?modulo=principal/carga/info_carga&acao=A&requisicao=downloadModeloArqExport'> clique aqui </a>.
            </td>
        </tr>
        <tr>
            <td>
                <b> 3. EXEMPLO DE ESTRUTURA DO ARQUIVO </b>
                <span style= "text-align:justify " >
                    <p>Segue abaixo uma relação dos campos que compõem o arquivo CSV. A estrutura dos campos não devem ser alterados em hipótese alguma como já citado. </p>
                </span>
            </td>
        </tr>    
        <tr>
            <td> <b> 3.1 ESTRUTURA DO ARQUIVO CSV </b> </td>
        </tr>
        <tr>
            <td>
                <table cellspacing= "0 " cellpadding= "2" border= "0" align= "center" width= "98%" class="tabela listagem">
                    <tr>
                        <td class= "cabecalho title" > Matrícula SIAPE </td>
                        <td class= "cabecalho title" > Nome do servidor  </td>
                        <td class= "cabecalho title" > Código do Orgão </td>
                        <td class= "cabecalho title" > CPF </td>
                        <td class= "cabecalho title" > Data de nascimento </td>
                        <td class= "cabecalho title" > Tipo </td>
                    </tr>
                    <tr style="background-color: #CDC9C9; ">
                        <td> 1234567 </td>
                        <td> José da Silva  </td>
                        <td> 12345 </td>
                        <td> 27214490154 </td>
                        <td> 10/05/1972 </td>
                        <td> AP </td>
                    </tr>
                    <tr style="background-color: #FFFAFA; ">
                        <td> 7654321 </td>
                        <td> Maria da Silva  </td>
                        <td> 12345 </td>
                        <td> 57214490154 </td>
                        <td> 31/01/1977 </td>
                        <td> AP </td>
                    </tr>
                    <tr style="background-color: #CDC9C9; ">
                        <td> 1234567 </td>
                        <td> João da Silva  </td>
                        <td> 12345 </td>
                        <td> 97214490154 </td>
                        <td> 15/12/1975 </td>
                        <td> AT </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>