<?php

$agdid = ( $_REQUEST['agdid'] ? $_REQUEST['agdid'] : $_SESSION['agenda']['agdid']);
$_SESSION['agenda']['agdid'] = $agdid;

switch( $_REQUEST['op'] ){
	
	case 'salvar':
		
		unset( $_POST['op']);
				
		$parametroagenda = new ParametroAgenda();

		$del  = $parametroagenda->deletaTodosPorAgenda($_SESSION['agenda']['agdid']);
		
		$save = $parametroagenda->salvar($_SESSION['agenda']['agdid']);
		
		$db->commit();
		
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/parametrosAgenda&acao=A\';
			 </script>');
}

if ( empty($agdid) ){
	die('<script>
			alert(\'Faltam parametros para acessar a tela.\');
			location.href = \'?modulo=inicio&acao=C\';
		 </script>');
}

$agenda = new Agenda( $agdid );

if ( !$agenda->possuiPermissaoAgenda( $agdid ) ){
	die('<script>
			alert(\'O usuário não possui permissão de acesso a agenda!\');
			location.href=\'?modulo=principal/listaAgenda&acao=A\';
		 </script>');		
}

$dados  = $agenda->getDados();
extract( $dados );	

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
//if ( $_REQUEST['acao'] == 'E' ){
//}
$arAba = abaEdicaoEvento();
echo montarAbasArray($arAba, "?modulo=principal/parametrosEvento&acao=A");
monta_titulo( 'Parâmetros da Agenda', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validaForm(){
	var errorText = new Array();
	
	$('[name=op]').val('salvar');
	$('[name=parametroEvento]').submit();
}

//-->
</script>

<?php

$parametroagenda = new ParametroAgenda();

$dadosconsulta = $parametroagenda->carregaDados($_SESSION['agenda']['agdid']);

$dadosparametroagenda = array();

foreach( $dadosconsulta as $arr ){

	array_push($dadosparametroagenda, $arr['pradsccampo'] );
}



?>

<form method="post" id="parametroEvento" name="parametroEvento" enctype="multipart/form-data" action="">	
	<input type="hidden" name="op" value=""/>	
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%; font-size:12pt;">Campos do Evento: <br /><br />
				<span style="font-size:8pt; text-decoration: inherit;">Marque os campos obrigatórios</span>
			</td>
			<td>
			 
				<input type="checkbox" name="tpeid" <?php echo $parametroagenda->verificaParametroAgenda( 'tpeid', $dadosparametroagenda ); ?>/>
				Tipo: <br />
				<input type="checkbox" name="tppid" <?php echo $parametroagenda->verificaParametroAgenda( 'tppid', $dadosparametroagenda ); ?>/>
				Prioridade: <br />
				<input type="checkbox" name="sltid" <?php echo $parametroagenda->verificaParametroAgenda( 'sltid', $dadosparametroagenda ); ?>/>
				Solicitante: <br />
				<input type="checkbox" name="cargo" <?php echo $parametroagenda->verificaParametroAgenda( 'cargo', $dadosparametroagenda ); ?>/>
				Cargo: <br />
				<input type="checkbox" name="etaid" <?php echo $parametroagenda->verificaParametroAgenda( 'etaid', $dadosparametroagenda ); ?>/>
				Entidade/Pessoa: <br />
				<input type="checkbox" name="prtid" <?php echo $parametroagenda->verificaParametroAgenda( 'prtid', $dadosparametroagenda ); ?>/>
				Participante(s): <br />
				<input type="checkbox" name="cevid" <?php echo $parametroagenda->verificaParametroAgenda( 'cevid', $dadosparametroagenda ); ?>/>
				Contato(s): <br />
				<input type="checkbox" name="evedtinicio" <?php echo $parametroagenda->verificaParametroAgenda( 'evedtinicio', $dadosparametroagenda ); ?>/>
				Data Início: <br />
				<input type="checkbox" name="evehsinicio" <?php echo $parametroagenda->verificaParametroAgenda( 'evehsinicio', $dadosparametroagenda ); ?>/>
				Hora Início: <br />
				<input type="checkbox" name="evedtfinal" <?php echo $parametroagenda->verificaParametroAgenda( 'evedtfinal', $dadosparametroagenda ); ?>/>
				Data Fim: <br />
				<input type="checkbox" name="evehsfinal" <?php echo $parametroagenda->verificaParametroAgenda( 'evehsfinal', $dadosparametroagenda ); ?>/>
				Hora Fim: <br />
				<input type="checkbox" name="estuf" <?php echo $parametroagenda->verificaParametroAgenda( 'estuf', $dadosparametroagenda ); ?>/>
				UF: <br />
				<input type="checkbox" name="muncod" <?php echo $parametroagenda->verificaParametroAgenda( 'muncod', $dadosparametroagenda ); ?>/>
				Município: <br />
				<input type="checkbox" name="evebairro" <?php echo $parametroagenda->verificaParametroAgenda( 'evebairro', $dadosparametroagenda ); ?>/>
				Bairro: <br />
				<input type="checkbox" name="evelogradouro" <?php echo $parametroagenda->verificaParametroAgenda( 'evelogradouro', $dadosparametroagenda ); ?>/>
				Logradouro: <br />
				<input type="checkbox" name="evenumero" <?php echo $parametroagenda->verificaParametroAgenda( 'evenumero', $dadosparametroagenda ); ?>/>
				Número: <br />
				<input type="checkbox" name="evecomplemento" <?php echo $parametroagenda->verificaParametroAgenda( 'evecomplemento', $dadosparametroagenda ); ?>/>
				Complemento: <br />
				<input type="checkbox" name="eveassunto" <?php echo $parametroagenda->verificaParametroAgenda( 'eveassunto', $dadosparametroagenda ); ?>/>
				Assunto: <br />
			</td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<input type="button" name="botao" value="Salvar" onclick="validaForm();" <?=($boBloqueioAba ? "disabled=\"disabled\"" : "" ) ?>/>
			</td>
		</tr>
	</table>
</form>
