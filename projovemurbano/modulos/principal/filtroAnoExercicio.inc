<?php
if ($_REQUEST['anoexercicio']) {
// 	ver($_REQUEST,d);
    $arDados = explode('_', $_REQUEST['anoexercicio']);
    $_SESSION['projovemurbano']['ppuid'] = $arDados[0];
    $_SESSION['projovemurbano']['ppuano'] = $arDados[1];
    
    echo "
    	<script>
	    	window.location='projovemurbano.php?modulo=inicio&acao=C';
    	</script>";
    die();
}
?>
<script>
    function selecionaAnoExercicio(ano) {
        if (ano != 'undefined') {
            //document.location.href = 'projovemurbano.php?modulo=inicio&acao=C&anoexercicio='+ano;
            document.getElementById('formAnoExercicio').submit();
        }
    }
</script>
<br/>
<form id="formAnoExercicio" name="formAnoExercicio" method="post" action="">
    <table class="tabela" width="95%" cellpadding="3" cellspacing="1" align="center">
        <tr>
            <td class="subtituloDireita" width="200">Ano de Execução do Projeto:</td>
            <td>
                <?php
                    $sql = "
                        select  ppuid || '_' || ppuano as codigo, 
                                ppuano as descricao 
                        from projovemurbano.programaprojovemurbano 
                        order by ppuano
                    ";
                    $anoexercicio = $_SESSION['projovemurbano']['ppuid'] . '_' . $_SESSION['projovemurbano']['ppuano'];

                    $db->monta_combo('anoexercicio', $sql, 'S', 'Selecione..', 'selecionaAnoExercicio', '', '', '', '', '', '', $anoexercicio);
                ?>		
            </td>
        </tr>
    </table>
</form>