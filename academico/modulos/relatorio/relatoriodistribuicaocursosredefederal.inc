<?php
/**
 * Sistema Integrado de Monitoramento do Ministério da Educação
 * Setor responsvel: SPO/MEC
 * Desenvolvedor: Desenvolvedores Simec
 * Módulo: Rede Federal
 * Finalidade: Gerar Relatório de consolidação de dados.
 * Data de criação: 24/11/2011
 * Última modificação: 24/11/2011
 */

if (( isset($_REQUEST["xls"]) ) && ( $_REQUEST["xls"] == 1 )) {
    header('Content-type: application/x-msdownload');
    header('Content-Disposition: attachment; filename=planilha_cursos_federais.xls');
    header('Pragma: no-cache');
    header('Expires: 0');

    # Gerando o XSL
    exibeXSLInstituicao();
}

if( empty($_REQUEST['ano'])) {
    $_REQUEST['ano']    = array();
}

if (isset($_REQUEST["email"]) && $_REQUEST["email"] == 1 && isset($_REQUEST["campoemail"])) {
    $conteudo = '
          <table width="95%" align="center" border="0" cellspacing="0" cellpadding="0" class="listagem">
                <thead>
                    <tr>
                        <td rowspan="2" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                            <strong>Instituição</strong>
                        </td>
                        <td rowspan="2" width="200" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                            <strong>Curso</strong>
                        </td>
                        <td colspan="5" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                            <center><strong>Provimentos Autorizados/Ano</strong></center>
                        </td>
                    </tr>
                    <tr>    
                        <td width="100" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                            <center>2008</center>
                        </td>
                        <td width="100" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                            <center>2009</center>
                        </td>
                        <td width="100" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                            <center>2010</center>
                        </td>
                        <td width="100" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                            <center>2011</center>
                        </td>
                        <td width="100" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                            <center>2012</center>
                        </td>
                    </tr>
                </thead>
                <tbody>';

    # Captura array de entidade e nivel de curso
    $arrNivelCurso = getNivel();
    $arrCoEntidade = ( is_array($_REQUEST["entid"]) ) ? $_REQUEST["entid"] : FALSE;

    # Percorre array de entidade
    foreach ($arrCoEntidade as $intCoEntidade) {
        # Captura o codigo e o nome da entidade
        $strNoEntidade = $intCoEntidade . " - " . getEntidadeById($intCoEntidade);

        $conteudo .= '<tr>
                                <td valign=top class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                    ' . $strNoEntidade . '
                                </td>
                                <td valign="top" width="700" colspan="6" style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">
                                    <table align="center" border="0" cellspacing="0" cellpadding="2" style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">
                                        <tbody>';

        # Captura array de curso de uma entidade
        $arrCurso = getCursoByEntidade($intCoEntidade);

        # Cria array do subtotal
        $arrSubTotalLancamento = array();

        # Percorre array de nivel de curso
        foreach ($arrNivelCurso as $nivelCurso) {

            $conteudo .='<tr bgcolor="#EFEFEF">
                                                    <td valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" colspan="6">
                                                        ' . $nivelCurso["nvcdsc"] . '
                                                    </td>
                                                </tr>';

            # Percorre array de curso
            foreach ($arrCurso as $curso) {
                # Verifica se o curso e do nivel atual
                if ($nivelCurso["nvcid"] == $curso["nvcid"]) {
                    # Captura array de lancamento
                    $intCoCurso = $curso["cpgid"];
                    $arrLancamento = getLancamentoByCurso($intCoCurso);

                    if (is_array($arrLancamento)) {
                        $intCountAno = 2008;
                        $arrLancamentoIntern = array();
                        foreach ($arrLancamento as $lancamento) {
                            $arrLancamentoIntern[$intCountAno] = ( $lancamento["lctano"] == $intCountAno ) ? $lancamento["lctvalor"] : 0;
                            $intCountAno++;
                        }
                    }
                    else
                        $arrLancamentoIntern = array("2008" => 0, "2009" => 0, "2010" => 0, "2011" => 0, "2012" => 0);

                    # Calcula os subtotais
                    $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2008"] += $arrLancamentoIntern["2008"];
                    $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2009"] += $arrLancamentoIntern["2009"];
                    $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2010"] += $arrLancamentoIntern["2010"];
                    $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2011"] += $arrLancamentoIntern["2011"];
                    $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2012"] += $arrLancamentoIntern["2012"];

                    $conteudo .='
                                                        <tr>
                                                            <td width="210" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                                ' . $curso["cpgcodigo"] . " - " . $curso["cpgdsc"] . '
                                                            </td>
                                                            <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                                <center>' . $arrLancamentoIntern["2008"] . '</center>
                                                            </td>
                                                            <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                                <center>' . $arrLancamentoIntern["2009"] . '</center>
                                                            </td>
                                                            <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                                <center>' . $arrLancamentoIntern["2010"] . '</center>
                                                            </td>
                                                            <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                                <center>' . $arrLancamentoIntern["2011"] . '</center>
                                                            </td>
                                                            <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                                <center>' . $arrLancamentoIntern["2012"] . '</center>
                                                            </td>
                                                        </tr>';
                }
            }
            $conteudo .= '<tr>
                                                    <td width="210" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;">
                                                        <b>SubTotal</b>
                                                    </td>
                                                    <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                        <center>' . $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2008"] . '</center>
                                                    </td>
                                                    <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                        <center>' . $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2009"] . '</center>
                                                    </td>
                                                    <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                        <center>' . $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2010"] . '</center>
                                                    </td>
                                                    <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                        <center>' . $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2011"] . '</center>
                                                    </td>
                                                    <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                        <center>' . $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2012"] . '</center>
                                                    </td>
                                                </tr>';

            # Calcula os totais
            $intTotal2008 += $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2008"];
            $intTotal2009 += $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2009"];
            $intTotal2010 += $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2010"];
            $intTotal2011 += $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2011"];
            $intTotal2012 += $arrSubTotalLancamento[$nivelCurso["nvcid"]]["2012"];
        }
        $conteudo .='
                                            <tr>
                                                <td width="210" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;">
                                                    <b>Total</b>
                                                </td>
                                                <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                    <center>' . $intTotal2008 . '</center>
                                                </td>
                                                <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                    <center>' . $intTotal2009 . '</center>
                                                </td>
                                                <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                    <center>' . $intTotal2010 . '</center>
                                                </td>
                                                <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                    <center>' . $intTotal2011 . '</center>
                                                </td>
                                                <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                                    <center>' . $intTotal2012 . '</center>
                                                </td>
                                            </tr>
                                        </tbody>
                                    </table>
                                </td>
                            </tr>
                        ';
                        }
               $conteudo .= '</tbody>
            </table>';

    $arquivoXls = '/tmp/' . uniqid() . 'planilha_cursos_federais.xls';
    $file = fopen($arquivoXls, 'w');
    
    fwrite($file, $conteudo);
    
    //file_put_contents($arquivoXls, exibeXSLInstituicao());
    //var_dump($arquivoXls);
      
    $arrayEmail = explode(',', $_REQUEST["campoemail"]);

    // carrega as funções gerais
    include_once "../../global/config.inc";
    include_once APPRAIZ . "includes/classes_simec.inc";
    include_once APPRAIZ . "includes/funcoes.inc";
    date_default_timezone_set('America/Sao_Paulo');

    // CPF do administrador de sistemas
    //if(!$_SESSION['usucpf'])
    //$_SESSION['usucpforigem'] = '';

    /*
     * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
     */
    require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
    require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
    $mail = new PHPMailer();

    $mail->SetLanguage("br", "libs/"); // ajusto a lingua a ser utilizadda
    $mail->SMTP_PORT = "587"; // ajusto a porta de smt a ser utilizada. Neste caso, a 587 que o GMail utiliza
    $mail->SMTPSecure = "tls"; // ajusto o tipo de comunicação a ser utilizada, no caso, a TLS do GMail

    $mail->IsSMTP(); // ajusto o email para utilizar protocolo SMTP
    $mail->SMTPSecure = 'ssl';
    $mail->Host = "localhost";


//			$mensagem->Mailer       = "smtp";
//			$mensagem->FromName		= !$this->name ? "SCDP" : $this->name;
//			$mensagem->From 		= $this->email_origem ;
//			$mensagem->Subject      = $this->title;
//			$mensagem->Body 		= $this->text;
//			$mensagem->IsHTML( true );

    $mail->SMTPAuth = true; // ativo a autenticação SMTP, no caso do GMail, é necessário
    //Aqui algumas informações que serão enviadas no cabeçalho do email
    $mail->From = "reuni@emec.gov.br"; // Email de quem envia o email
    $mail->FromName = "CGEG / SESu"; // Nome de quem envia o email

    foreach ($arrayEmail as $email) {
        $mail->AddAddress($email);
    }

    $mail->AddReplyTo("reuni@emec.gov.br", "CGEG / SESu"); // Email e nome que será utilizado quando a pessoa responder este email
    //$mail->WordWrap = 50; // quebra linha sempre que uma linha atingir 50 caracteres
    $mail->AddAttachment($arquivoXls); // adc arquivo anexo. *opcional*
    $mail->IsHTML(true); // ajusto envio do email no formato HTML

    $mail->Subject = "Cotas de Bolsas REUNI";
    $mail->Body = "Encaminhamos anexo com relatório das cotas de bolsas REUNI de<br />
                       Assistência ao Ensino referentes ao mês em curso, conforme<br />
                       distribuição no SIMEC por IFES, por Nível e por Programa de<br />
                       Pós-Graduação.<br /><br />
                       Equipe CGEG / SESu";
    if (!$mail->Send()) {
        unset($arquivoXls);
        echo "<script> alert('E-mail não pode ser enviado!'); </script>";
        //echo "Erro: " . $mail->ErrorInfo;
    }else{
        echo "<script> alert('E-mail enviado com sucesso!'); </script>";
    }

    unset($arquivoXls);
    
}
?>
<?php include APPRAIZ . "includes/cabecalho.inc"; ?>
<br />
<?php monta_titulo("Relat&oacute;rio - Plano de Distribui&ccedil;&atilde;o de Bolsas", ""); ?>
<style>
    #scroll
    {
        width:100%;
        height:150px;
        background-color:#F2F2F2;
        overflow:auto;
    }
</style>
<form method="POST" name="formulario" id="formulario">
    <input type="hidden" id="xls" name="xls">
    <input type="hidden" id="email" name="email">
    <input type="hidden" id="enviado" name="enviado">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
<?php
//# Flg que indica se o relatorio vai ou nao ser exibido
$booExibeRelatorio = FALSE;

# Verifica se foi submetido
if ($_REQUEST["enviado"] == "true") {
    global $db;
    # Capturando dados necessarios
    $usucpf = $_SESSION["usucpf"];
    $arrEntId = $_REQUEST["entid"];
    
    # Validacoes
    if (( is_null($usucpf) ) || ( empty($usucpf) ) || (!is_array($arrEntId) ) || ( count($arrEntId) == 0 ))
        echo "<script>alert('Ocorreu um erro e a operação não foi realizada.');</script>";
    else {
        # Exclui o registro antigo
        $strQuery = "DELETE FROM entidade.tb_entidade_relatorio WHERE usucpf = '" . pg_escape_string($usucpf) . "'";
        $resultado = $db->executar($strQuery);

        # Percorre o array de entidade salvando no banco de dados
        foreach ($arrEntId as $entId) {
            $strQuery = "INSERT INTO entidade.tb_entidade_relatorio ( usucpf , entid ) VALUES ( '" . pg_escape_string($usucpf) . "' , " . pg_escape_string($entId) . " )";
            $resultado = $db->executar($strQuery);
        }
        $booExibeRelatorio = TRUE;
    }
}

$sql = "SELECT DISTINCT
                                e.entid , e.entnome
                            FROM 
                                entidade.entidade e
                            INNER JOIN academico.cursoposgraduacao AS sc ON
                                sc.entid = e.entid
                            WHERE  
                                entstatus = 'A'
                            ORDER BY entnome";
$resultado = $db->carregar($sql);
if (is_array($resultado)) {
    # Recupera o ultimo relatorio gerado pelo usuario
    $strQuery = "SELECT entid FROM entidade.tb_entidade_relatorio WHERE usucpf = '" . pg_escape_string($_SESSION["usucpf"]) . "'";
    $arrEntidadeRelatorio = $db->carregar($strQuery);

    # Cria um array com id da entidade para marcar ou nao o checkbox
    $arrEntRelatorioId = array();
    if (is_array($arrEntidadeRelatorio)) {
        foreach ($arrEntidadeRelatorio as $intCount => $entidadeRelatorio) {
            $arrEntRelatorioId[$intCount] = $entidadeRelatorio["entid"];
        }
    }
}
                $stSql = "SELECT DISTINCT
                                e.entid AS codigo , e.entnome AS descricao
                            FROM 
                                entidade.entidade e
                            INNER JOIN academico.cursoposgraduacao AS sc ON
                                sc.entid = e.entid
                            WHERE  
                                entstatus = 'A'
                            ORDER BY descricao";
                
                $stSqlCarregados = "";
                
                if( !empty($_REQUEST['entid']) ){
                    
                    $stSqlCarregados = "SELECT DISTINCT
                                    e.entid AS codigo , e.entnome AS descricao
                                FROM 
                                    entidade.entidade e
                                INNER JOIN academico.cursoposgraduacao AS sc ON
                                    sc.entid = e.entid
                                WHERE  
                                    entstatus = 'A'
                                AND e.entid in ( ". implode( ',', $_REQUEST['entid']) ." )
                                ORDER BY descricao";
                }
                
		mostrarComboPopup( 'Instituição', 'entid',  $stSql, $stSqlCarregados, 'Selecione a(s) Instituição(ões)' ); 
?>
        </tr>
        <tr>
            <?php
                 $stSqlAno   = "SELECT DISTINCT lctano AS codigo, lctano AS descricao 
                                    FROM academico.lancamento 
                                    ORDER BY lctano";
            
                $stSqlCarregadosAno = "";
                
                if( !empty($_REQUEST['ano']) )
                {
                    $stSqlCarregadosAno = "SELECT DISTINCT l.lctano AS codigo, l.lctano AS descricao 
                                    FROM academico.lancamento l
                                    WHERE l.lctano IN ( '". implode( "','", $_REQUEST['ano']) ."' )
                                    ORDER BY lctano";
                }
                mostrarComboPopup( 'Ano', 'ano',  $stSqlAno, $stSqlCarregadosAno, 'Selecione o(s) ano(s)' ); 
            ?>
            
        </tr>
        <tr id="tr-email" style="display:none" bgcolor="#C0C0C0">
            <td colspan="3" valign="top" align="right">
                <center>
                    Obs: Separe os e-mails por vírgulas e sem espaços.</br>
                    <input size="50px" type="text" name="campoemail" id="campoemail"><br>
                    <input type='button' class="botao" name='cancel-envia-email' value='Cancelar' onclick="cancelarExibeCampoEmail();" /> <input type='button' class="botao" name='envia-email' value='Enviar e-mail' onclick="enviaEmail();" />
                </center>
            </td>
        </tr>
        <tr bgcolor="#C0C0C0">
            <td colspan="3" valign="top" align="right">	
                <center>
                    <?php 
                    if ($_REQUEST["enviado"] == "true"): ?>
                        <input type='button' class='botao' name='xls' id='xls' value='Gerar XLS' onclick='funtionExportarRel();' />
                        <input type='button' class='botao' name='exibe-campo-email' id='email' value='Enviar por e-mail' onclick='exibeCampoEmail();' />
                    <?php endif ?>
                    <input type='button' class="botao" name='consultar' value='Consultar' onclick="enviar();" />
                </center>
            </td>
        </tr>
    </table>
</form>
<br />
<?php
if ($booExibeRelatorio == TRUE)
    exibeTelaInstituicao();

/**
 * Gera o relatorio em XLS
 * 
 * @global type $db
 */
function exibeXSLInstituicao() {
    exibeTelaInstituicaoXLS();
    die;
}

// Fim do XLS

/**
 * Retorna os niveis dos cursos
 * 
 * @global DB $db
 * @return ARRAY $result
 */
function getNivel() {
    global $db;
    $sql = "SELECT * FROM academico.nivelcurso";
    $result = $db->carregar($sql);
    return ( $result ) ? $result : null;
}

/**
 * Retorna informacoes da entidade
 * 
 * @global DB $db
 * @param INTEGER $id
 * @return ARRAY $result
 */
function getEntidadeById($id) {
    global $db;
    $sql = "SELECT entnome FROM entidade.entidade WHERE entid = '" . $id . "'";
    $result = $db->pegaUm($sql);
    return ( $result ) ? $result : null;
}

/**
 * Retorna curso da entidade
 * 
 * @global DB $db
 * @param INTEGER $entid
 * @return ARRAY $result 
 */
function getCursoByEntidade($entid) {
    global $db;
    $sql = "SELECT * FROM academico.cursoposgraduacao WHERE entid = '" . $entid . "'";
    $result = $db->carregar($sql);
    return ( $result ) ? $result : null;
}

/**
 * Retorna lancamento do curso
 * 
 * @global DB $db
 * @param INTEGER $entid
 * @return ARRAY $result 
 */
function getLancamentoByCurso($intCoCurso) {
    global $db;
    $sql = "SELECT * FROM academico.lancamento WHERE cpgid = '" . $intCoCurso . "'";
    $result = $db->carregar($sql);
    return ( $result ) ? $result : null;
}

/**
 * Exibe o relatorio na tela
 * 
 * @return VOID
 */
function exibeTelaInstituicao() {
    ?>
    <table width="95%" align="center" border="0" cellspacing="0" cellpadding="0" class="listagem">
        <thead>
            <tr>
                <td rowspan="2" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                    <strong>Instituição</strong>
                </td>
                <td rowspan="2" width='200' valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                    <strong>Curso</strong>
                </td>
                <td colspan="7" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
        <center><strong>Provimentos Autorizados/Ano</strong></center>
    </td>
    </tr>
    <tr>
        <?php 
        $todosOsAnos    = array('2008','2009','2010','2011','2012','2013','2014');
        foreach( $todosOsAnos as $ano ) {
            if(in_array($ano, $_REQUEST['ano']) ) { ?>
                <td width='100' valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                    <center><?php echo $ano ?></center>
                </td>
            <?php
            }
        }
        ?>
    </tr>
    </thead>
    <tbody>
    <?php
    # Captura array de entidade e nivel de curso
    $arrNivelCurso = getNivel();
    $arrCoEntidade = ( is_array($_REQUEST['entid']) ) ? $_REQUEST['entid'] : FALSE;

    # Percorre array de entidade
    foreach ($arrCoEntidade as $intCoEntidade) {
        # Captura o codigo e o nome da entidade
        $strNoEntidade = $intCoEntidade . " - " . getEntidadeById($intCoEntidade);
        ?>

            <tr>
                <td valign=top class="title"  style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
        <?php echo $strNoEntidade; ?>
                </td>
                <td valign="top"  colspan="8" style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">
                    <table align="center" border="0" cellspacing="0" cellpadding="2" style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">
                        <tbody>
        <?php
        # Captura array de curso de uma entidade
        $arrCurso = getCursoByEntidade($intCoEntidade);
        # Cria array do subtotal
        $arrSubTotalLancamento = array();
        

        # Percorre array de nivel de curso
        foreach ($arrNivelCurso as $nivelCurso) {
            ?>

                                <tr bgcolor="#EFEFEF">
                                    <td valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title" colspan="8">
            <?php echo $nivelCurso["nvcdsc"]; ?>
                                    </td>
                                </tr>
                        <?php
                        # Percorre array de curso
                        foreach ($arrCurso as $curso) {
                            # Verifica se o curso e do nivel atual
                            if ($nivelCurso["nvcid"] == $curso["nvcid"]) {
                                # Captura array de lancamento
                                $intCoCurso = $curso["cpgid"];
                                $arrLancamento = getLancamentoByCurso($intCoCurso);

                                if (is_array($arrLancamento)) {
                                    $intCountAno = 2008;
                                    $arrLancamentoIntern = array();
                                    foreach ($arrLancamento as $lancamento) {
                                        $arrLancamentoIntern[$intCountAno] = ( $lancamento["lctano"] == $intCountAno ) ? $lancamento["lctvalor"] : 0;
                                        $intCountAno++;
                                    }
                                }
                                else
                                    $arrLancamentoIntern = array();
                                    foreach( $_REQUEST['ano'] as $anoSelecionado )
                                    {
                                        $arrLancamentoIntern[$anoSelecionado] = 0;
                                    }
                                    
                                    foreach( $_REQUEST['ano'] as $anoSelecionado )
                                    {
                                        $arrSubTotalLancamento[$nivelCurso["nvcid"]][$anoSelecionado] += $arrLancamentoIntern[$anoSelecionado];
                                    }
                                ?>
                                        <tr>
                                            <td  style="text-wrap:normal; word-wrap:break-word; max-width: 210px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" >
                                        <?php echo $curso["cpgcodigo"] . " - " . $curso["cpgdsc"] ; ?>
                                            </td>
                            <?php 
                            foreach( $_REQUEST['ano'] as $anoSelecionado ){ ?>
                                <td  style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                    <center><?php echo $arrLancamentoIntern[$anoSelecionado]; ?></center>
                                </td>
                            <?php
                            } 
                        }
                    }
                    ?>
            <tr>
                <td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;">
                    <b>SubTotal</b>
                </td>
                 <?php 
                foreach( $_REQUEST['ano'] as $anoSelecionado ){ ?>
                    <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                        <center><?php echo !empty($arrSubTotalLancamento[$nivelCurso["nvcid"]][$anoSelecionado]) ? $arrSubTotalLancamento[$nivelCurso["nvcid"]][$anoSelecionado] : 0  ; ?></center>
                    </td>
                <?php
                } 
                ?>
               
            </tr>
            <?php
            
            foreach( $_REQUEST['ano'] as $anoSelecionado ){
                $$anoSelecionado    += $arrSubTotalLancamento[$nivelCurso["nvcid"]][$anoSelecionado];
            } 
            
        }
        ?>
        <tr>
            <td width="210" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;">
                <b>Total</b>
            </td>
            
            <?php
            foreach( $_REQUEST['ano'] as $anoSelecionado ){ ?>
                <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                    <center><?php echo $$anoSelecionado; ?></center>
                </td>
           <?php
            } 
            ?>
            
        </tr>
        </tbody>
        </table>
        </td>
        </tr>
        <?php } ?>
    </tbody>
    </table>
<?php } 

function exibeTelaInstituicaoXLS() {
    ?>
    <table width="95%" align="center" border="0" cellspacing="0" cellpadding="0" >
        <thead>
            <tr>
                <td valign="top"  style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                    Instituição
                </td>
                <td width='200' valign="top"  style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                    Curso
                </td>
                <td colspan="5" valign="top"  style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                    <center>
                            Provimentos Autorizados/Ano
                    </center>
                </td>
            </tr>
    <tr>
        <td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">&nbsp;</td>
        <td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" >&nbsp;</td>
        <?php 
        $todosOsAnos    = array('2008','2009','2010','2011','2012','2013','2014');
        foreach( $todosOsAnos as $ano ) {
            if(in_array($ano, $_REQUEST['ano']) ) { ?>
                <td width='100' valign="top"  style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                    <center><?php echo $ano ?></center>
                </td>
            <?php
            }
        }
        ?>
    </tr>
    </thead>
    <tbody>
    <?php
    # Captura array de entidade e nivel de curso
    $arrNivelCurso = getNivel();
    $arrCoEntidade = ( is_array($_REQUEST["entid"]) ) ? $_REQUEST["entid"] : FALSE;
    
 //   var_dump( $_REQUEST['entid'] );exit;

    # Percorre array de entidade
    
    $controleExibeNome   = false;
    foreach ($arrCoEntidade as $intCoEntidade) {
        $controleExibeNome  = true;
        # Captura o codigo e o nome da entidade
        $strNoEntidade = $intCoEntidade . " - " . getEntidadeById($intCoEntidade);
        ?>

            <tr>
<!--                <td valign=top  style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
        <?php //echo $strNoEntidade; ?>
                </td>-->
                <td valign="top"  colspan="7" style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">
                    <table width="100%"  border="0" cellspacing="0" cellpadding="2" style="border-right: 0px solid #c0c0c0; border-bottom: 0px solid #c0c0c0; border-left: 0px solid #ffffff;">
                        <tbody>
        <?php
        # Captura array de curso de uma entidade
        $arrCurso = getCursoByEntidade($intCoEntidade);
        
        # Cria array do subtotal
        $arrSubTotalLancamento = array();
        
        # Percorre array de nivel de curso
        foreach ($arrNivelCurso as $nivelCurso) {
            ?>

                                <tr>
                                    <td style=" text-align: left; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" ><?php echo  $controleExibeNome === true ? $strNoEntidade : '&nbsp'; ?></td>
                                    <td valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" >
            <?php echo $nivelCurso["nvcdsc"]; ?>
                                    </td>
                                    <td style=" text-align: left; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" >&nbsp;</td>
                                    <td style=" text-align: left; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" >&nbsp;</td>
                                    <td style=" text-align: left; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" >&nbsp;</td>
                                    <td style=" text-align: left; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" >&nbsp;</td>
                                    <td style=" text-align: left; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" >&nbsp;</td>
                                </tr>
                        <?php
                        
                        # Percorre array de curso
                        foreach ($arrCurso as $curso) {
                            # Verifica se o curso e do nivel atual
                            if ($nivelCurso["nvcid"] == $curso["nvcid"]) {
                                # Captura array de lancamento
                                $intCoCurso = $curso["cpgid"];
                                $arrLancamento = getLancamentoByCurso($intCoCurso);

                                if (is_array($arrLancamento)) {
                                    $intCountAno = 2008;
                                    $arrLancamentoIntern = array();
                                    foreach ($arrLancamento as $lancamento) {
                                        $arrLancamentoIntern[$intCountAno] = ( $lancamento["lctano"] == $intCountAno ) ? $lancamento["lctvalor"] : 0;
                                        $intCountAno++;
                                    }
                                }
                                else
                                    $arrLancamentoIntern = array();
                                    foreach( $_REQUEST['ano'] as $anoSelecionado )
                                    {
                                        $arrLancamentoIntern[$anoSelecionado] = 0;
                                    }
                                    
                                    foreach( $_REQUEST['ano'] as $anoSelecionado )
                                    {
                                        $arrSubTotalLancamento[$nivelCurso["nvcid"]][$anoSelecionado] += $arrLancamentoIntern[$anoSelecionado];
                                    }
                                ?>
                                        <tr>
                                            <td style=" width: 530px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;" >&nbsp;</td>
                                            <td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                        <?php echo $curso["cpgcodigo"] . " - " . $curso["cpgdsc"]; ?>
                                            </td>
                            <?php 
                            foreach( $_REQUEST['ano'] as $anoSelecionado ){ ?>
                                <td  style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                                    <center><?php echo $arrLancamentoIntern[$anoSelecionado]; ?></center>
                                </td>
                            <?php
                            } 
                        }
                    }
                    ?>
                                </tr>
            <tr>
                <td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: left;" >&nbsp;</td>
                <td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: left;">
                    SubTotal
                </td>
                 <?php 
                foreach( $_REQUEST['ano'] as $anoSelecionado ){ ?>
                    <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                        <center><?php echo $arrSubTotalLancamento[$nivelCurso["nvcid"]][$anoSelecionado]; ?></center>
                    </td>
                <?php
                } 
                ?>
               
            </tr>
            <?php
            
            foreach( $_REQUEST['ano'] as $anoSelecionado ){
                $$anoSelecionado    += $arrSubTotalLancamento[$nivelCurso["nvcid"]][$anoSelecionado];
            } 
            $controleExibeNome  = false;
        }
        ?>
        <tr>
            <td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;">&nbsp;</td>
            <td width="210" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: left;">
                Total
            </td>
            
            <?php
            foreach( $_REQUEST['ano'] as $anoSelecionado ){ ?>
                <td width="105" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
                    <center><?php echo $$anoSelecionado; ?></center>
                </td>
           <?php
            } 
            ?>
            
        </tr>
        </tbody>
        </table>
        </td>
        </tr>
        <?php } ?>
    </tbody>
    </table>
<?php } ?>



<script type="text/javascript">
    
    function checkMail(mail){
        
        //mail = trim(mail);
        
        var er = new RegExp(/^[A-Za-z0-9_\-\.]+@[A-Za-z0-9_\-\.]{2,}\.[A-Za-z0-9]{2,}(\.[A-Za-z0-9])?/);
        if(typeof(mail) == "string"){
            if(er.test(mail)){ return true; }
        }else if(typeof(mail) == "object"){
            if(er.test(mail.value)){ 
                return true; 
            }
        }else{
            return false;
        }
    }
    
    function enviar()
    {
        document.getElementById( "xls" ).value      = 0; 
        document.getElementById( "enviado" ).value  = true;
        
        if (document.getElementById('entid').options.length >= 1) {
            if( document.getElementById('entid').options[0].value == '' ){
                alert(' Selecione ao menos uma instituição.');
                return false;
            }
            
        }
        if (document.getElementById('ano').options.length >= 1) {
            if( document.getElementById('ano').options[0].value == '' ){
                alert(' Selecione ao menos um ano.');
                return false;
            }
            

        }
        
        selectAllOptions( document.getElementById('formulario').entid );
        selectAllOptions( document.getElementById('formulario').ano );
        
        document.getElementById('formulario').submit();
    }
    
    function funtionExportarRel()
    {
        selectAllOptions( document.getElementById('formulario').entid );
        selectAllOptions( document.getElementById('formulario').ano );
        document.getElementById( "xls" ).value = 1; 
        formulario.submit();
    }
    
    function cancelarExibeCampoEmail()
    {
        document.getElementById('tr-email').style.display="none";
    }
    
    function exibeCampoEmail() 
    {
        document.getElementById('tr-email').style.display="";
    }
    
    function enviaEmail()
    {
        var emailValue = document.getElementById( "campoemail" ).value;
        
        if(emailValue == ""){
            alert('É necessário pelo menos um e-mail!')
            return;
        }else{
            
            var arrayEmail = emailValue.split(",");
            //alert(arrayEmail.length);
            
            for (i = 0; i < arrayEmail.length; i++) {
                if(!checkMail(arrayEmail[i])){
                    alert('O e-mail ' + arrayEmail[i] + ' não é válido!');
                    return;
                }
            }            
        }
        
        document.getElementById( "email" ).value = 1; 
        formulario.submit();
    }
        
/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
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
</script>
