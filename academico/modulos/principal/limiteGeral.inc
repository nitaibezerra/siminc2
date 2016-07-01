<?php

// cabecalho da app
require_once APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . "academico/classes/NivelCurso.class.inc";
require_once APPRAIZ . "academico/classes/LimiteGeral.class.inc";
require_once APPRAIZ . "academico/classes/Lancamento.class.inc";
require_once APPRAIZ . "academico/classes/LiberacaoIndividual.class.inc";
echo '<br/>';

$abacod_tela = ($_SESSION['baselogin'] == "simec_desenvolvimento") ? ABA_CURSO_POSGRADUACAO_DEV : ABA_CURSO_POSGRADUACAO_OLD; //Dados Desenv

$db->cria_aba( $abacod_tela , $url, array() );

monta_titulo("Limite de Cotas", "");

$oNivelCurso = new NivelCurso();
$oLimiteGeral = new LimiteGeral();
$oLiberacao = new LiberacaoIndividual();
$oLancamento = new Lancamento();

// recupera os niveis de curso cadastrados
$niveisCurso = $oNivelCurso->lista();

// extraindo dados para a tela
extract($_POST);
extract($_GET);

$entid = $entid ? $entid : $_SESSION['academico']['entid'];

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
echo $autoriazacaoconcursos->cabecalho_entidade($entid);

// se acionado o botao salvar
if(isset($_POST['btnSalvar'])){

	// salva as informações de limite geral
	$ltgid = $oLimiteGeral->salvar($ltg, $entid);
	
	if( $oLiberacao->validaPeriodoIndividual($libdtinicio, $libdtfinal) ){
		
		echo '<script> alert("O período de liberação individual não pode ser igual ao período de liberação geral"); </script>';
		
	}else{
	
	    // salva o periodo de liberacao
	    if( $oLiberacao->existe( $entid ) ){
	    	$oLiberacao->atualizar($libdtinicio, $libdtfinal, $entid);
	    }else{
	    	$oLiberacao->salvar($libdtinicio, $libdtfinal, $entid);
	    }
	    
	    echo '<script> alert("Salvo com Sucesso."); </script>';
	}

}else{

	// recupera os dados de limite individual
	$ltg = $oLimiteGeral->pegarDados($entid);

	if( count($ltg) == 0 ){
		// recupera os dados de limite geral
		$ltg = $oLimiteGeral->listaLimiteDeCotasGeral();
	}
	
    // recupera e formada o periodo e liberacao
    $dadosLiberacao = $oLiberacao->pegarDados( $entid );
    $libdtinicio 	= formata_data($dadosLiberacao['libindtinicio']);
    $libdtfinal 	= formata_data($dadosLiberacao['libindtfinal']);

}


?>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
$(document).ready(function(){

	$("#formulario").submit(function(){

		if( $("#libdtinicio").val() == "" &&  $("#libdtfinal").val() == ""){

			alert( 'Preencha os campos obrigatórios.' );
			return false;
			
		}else{

			var dataInicio  = $("#libdtinicio").val();
			var dataFim		= $("#libdtfinal").val();
			
			if( dataInicio == "" || dataFim == "" ){
				alert( 'Período inválido' );
				return false;
			}else{

			    var dataInicioConvertida = dataInicio.substring(6,10) + dataInicio.substring(3,5) + dataInicio.substring(0,2);
			    var dataFimConvertida    = dataFim.substring(6,10) + dataFim.substring(3,5) + dataFim.substring(0,2);
				
			    if (dataInicioConvertida > dataFimConvertida){
			        alert( 'Período "'+ dataInicio + ' a ' + dataFim + '" inválido' );
			        return false;
			    }
			}
		}
	});
});

</script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet">
<style type="text/css">
    
    .center {
        text-align: center;
    }
    
</style>

<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

    <table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <th rowspan="2" style="width:300px;"> N&iacute;vel </th>
            <th colspan="5"> Ano </th>
        </tr>
        <tr>
            <th> 2008 </th>
            <th> 2009 </th>
            <th> 2010 </th>
            <th> 2011 </th>
            <th> 2012 </th>
        </tr>
        <?php 
		$liberaGeral = ( $db->testa_superuser() ) ? 'S' : 'N';

        foreach($niveisCurso as $nivel): ?>
        <tr>
            <td class="SubTituloDireita">
                <?php echo $nivel['nvcdsc']; ?>:
            </td>
            <td class="center">
                <?php echo campo_texto("ltg[{$nivel['nvcid']}][2008]", 'N', $liberaGeral, '', 4, 4, '####', '', '', '', '', 'id="ltg2008"', '', $ltg[$nivel['nvcid']]['2008']); ?>
            </td>
            <td class="center">
                <?php echo campo_texto("ltg[{$nivel['nvcid']}][2009]", 'N', $liberaGeral, '', 4, 4, '####', '', '', '', '', 'id="ltg2008"', '', $ltg[$nivel['nvcid']]['2009']); ?>
            </td>
            <td class="center">
                <?php echo campo_texto("ltg[{$nivel['nvcid']}][2010]", 'N', $liberaGeral, '', 4, 4, '####', '', '', '', '', 'id="ltg2008"', '', $ltg[$nivel['nvcid']]['2010']); ?>
            </td>
            <td class="center">
                <?php echo campo_texto("ltg[{$nivel['nvcid']}][2011]", 'N', $liberaGeral, '', 4, 4, '####', '', '', '', '', 'id="ltg2008"', '', $ltg[$nivel['nvcid']]['2011']); ?>
            </td>
            <td class="center">
                <?php echo campo_texto("ltg[{$nivel['nvcid']}][2012]", 'N', $liberaGeral, '', 4, 4, '####', '', '', '', '', 'id="ltg2008"', '', $ltg[$nivel['nvcid']]['2012']); ?>
            </td>
        </tr>
        <?php endforeach; ?>
    </table>

    <?php monta_titulo("Liberação", ""); ?>

    <table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita" style="width:300px;"> Data: </td>
            <td>

                <?php echo campo_data2('libdtinicio', 'S', $liberaGeral, 'Informe a Data Inicial de Liberação', 'N', '', '', $libdtinicio);?>
                &nbsp;&nbsp; a &nbsp;&nbsp;
                <?php echo campo_data2('libdtfinal', 'S', $liberaGeral, 'Informe a Data Inicial de Liberação', 'N', '', '', $libdtfinal);?>
                
            </td>
        </tr>
        <tr>
            <th colspan="6" style="text-align: center;">
                <input type="hidden" value="<?php echo $entid; ?>" name="entid" id="entid" />
                <?php if( $db->testa_superuser() ): ?>
                <input type="submit" value="Salvar" name="btnSalvar" id="btnSalvar" />
                <?php endif; ?>
            </th>
        </tr>
    </table>

</form>