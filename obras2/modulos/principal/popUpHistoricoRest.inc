<?
$rstid = $_GET['rstid'];
?>
<html>
	<head>
		<title>Histórico Restrições/Inconformidades</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
		<link rel="stylesheet" type="text/css" media="screen, print" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	</head>
	<body topmargin="0" leftmargin="0">
        <table class="listagem" cellspacing="0" cellpadding="3" align="center" style="width: 100%;">
            <thead>
                <tr>
                    <td style="text-align: center; background-color: #e0e0e0;" colspan="6">
                        <b style="font-size: 10pt;">Histórico de Restrições/Inconformidades<br/></b>
                    </td>
                </tr>
            </thead>
        </table>
        <br>
		<?
        global $db;
        $historico = new HistoricoRestricao();
        $dados = $historico->listaHistorico($rstid);
        $cabecalho = array('ID','Responsável','Item','Data da Inclusão','Descrição','Providência','Previsão da Providência');
        $db->monta_lista($dados,$cabecalho,100,5,'N','center','N', 'N');
        ?>
        <br>
        <div style="text-align: center;">
            <input class="botao" type="button" value="Fechar" onclick="window.close();">
        </div>
	</body>
</html>