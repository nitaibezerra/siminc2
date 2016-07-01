<?php
	if ($_POST['colunas']){
		ini_set("memory_limit","256M");
		include("result_relatorio_avaliacao.inc");
		exit;
	}
	
	include APPRAIZ . 'includes/Agrupador.php';
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';	

	monta_titulo( 'Sistema de Getão de Pessoas - Módulo Avaliação', 'Relatório - Avaliação Final' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório - Avaliação Final</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<script type="text/javascript">

	function gerarRelatorio(tipo){
		var formulario = document.formulario;
		var tipo_relatorio = tipo;

		selectAllOptions( formulario.colunas );

		if(tipo_relatorio == 'visual'){
			document.getElementById('tipo_relatorio').value = 'visual';
		}else{
			document.getElementById('tipo_relatorio').value = 'xls';
		}
		
		var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';
		formulario.submit();
		
		janela.focus();
	}
	
	function onOffCampo( campo ){
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
        
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}else{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
</script>

</head>
<body>
    
<form name="formulario" id="formulario" action="" method="POST">
	<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>
	
	<table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" width="10%">Colunas</td>
			<td>
				<?php				
					#Montar e exibe colunas
					$arrayColunasOrig = colunasOrigem();
					$arrayColunasDest = colunasDestino();
					$colunas = new Agrupador('formulario');
					$colunas->setOrigem('naoColunas', null, $arrayColunasOrig);
					$colunas->setDestino('colunas', null, $arrayColunasDest);
					$colunas->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloesquerda" colspan="2">
				<strong>Filtros</strong>
			</td>
		</tr>
        <tr>
            <td class="subtitulodireita">Status</td>
            <td>
                <?php
                    $sql = array(
                        array("codigo" => "t", "descricao" => "Não finalizada"),
                        array("codigo" => "f", "descricao" => "Finalizada")
                    );
                    $db->monta_combo("resavaliacaopendente", $sql,'S', 'Selecione o Status...', '', '','Selecione o item caso queira filtrar por Status!', '435');		
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" >&nbsp;</td>
            <td lign="center">
                <input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('visual');"/>
                <input type="button" name="Gerar Relatório" value="Visualizar XLS" onclick="javascript:gerarRelatorio('xls');"/>
            </td>
        </tr>
	</table>
</form>
</body>
</html>

<?php
	function colunasOrigem(){	
		return array(
            array(
				'codigo'    => 'sersiape',
				'descricao' => '01. SIAPE'
			),
            /*
			array(
				'codigo'    => 'sercpf',
				'descricao' => '02. CPF'
			),
             * 
             */			
			array(
				'codigo'    => 'sernome',
				'descricao' => '02. Servidor'
			),			
			array(
				'codigo'    => 'soma_nota_auto',
				'descricao' => '03. Nota Auto-Avaliação'
			),
			array(
				'codigo'    => 'soma_nota_sup',
				'descricao' => '04. Nota Superior'
			),
			array(
				'codigo'    => 'soma_nota_eqp',
				'descricao' => '05. Nota Equipe'
			),
			array(
				'codigo'    => 'avaliacao_final',
				'descricao' => '06. Avaliação Final'
			),
			array(
				'codigo'    => 'pontos',
				'descricao' => '07. Pontuação'
			),
			array(
				'codigo'    => 'falta_nota',
				'descricao' => '08. Status' 
			),
			array(
				'codigo'    => 'resavaliacaopendente',
				'descricao' => '09. Situação'
			)					
		);		
	}
	
    function colunasDestino(){
        return array(
            
			array(
				'codigo'    => 'sersiape',
				'descricao' => '01. SIAPE'
			),
			array(
				'codigo'    => 'sernome',
				'descricao' => '02. Servidor'
			),
			array(
				'codigo'    => 'avaliacao_final',
				'descricao' => '06. Avaliação Final'
			),
			array(
				'codigo'    => 'pontos',
				'descricao' => '07. Pontuação'
			),
			array(
				'codigo'    => 'resavaliacaopendente',
				'descricao' => '09. Situação'
			)					
		);        
    }
    	
?>