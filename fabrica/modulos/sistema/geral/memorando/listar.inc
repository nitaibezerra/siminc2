<?php
header('content-type: text/html; charset=iso-8859-1;');

include APPRAIZ . 'includes/cabecalho.inc';


$arAtributos = array(
		'tpdpsid'         => null,
		'tpdpsdsc'        => null,
		'tpdpsdtcadastro' => null,
		'tpdpsstatus'     => null
);

print "<br/>";
monta_titulo("Memorando", '');

$memorandoRepositorio   = new MemorandoRepositorio();
$despesaRepositorio		= new TipoDespesaRepositorio();
$perfis                 = arrayPerfil();

$tipoMemorando          = $_POST['tipoMemorando'];
$tipoDespesa			= $_POST['tipoDespesa'];
$anoMemorando           = $_POST['anomemorando'];

$sqlMemorandos			= $memorandoRepositorio->recuperaMemoPorFiltro( $perfis, $tipoMemorando ,  $tipoDespesa, $anoMemorando );

//váriaves utilizados em www/fabrica/geral/memorando/listar.php
$_SESSION['perfis']                                 = $perfis;
$_SESSION['MEMO_PERFIL_FISCAL_CONTRATO']            = PERFIL_FISCAL_CONTRATO;
$_SESSION['MEMO_PERFIL_ESPECIALISTA_SQUADRA']       = PERFIL_ESPECIALISTA_SQUADRA;
$_SESSION['MEMO_PERFIL_SUPER_USUARIO']              = PERFIL_SUPER_USUARIO;
$_SESSION['MEMO_PERFIL_ADMINISTRADOR']              = PERFIL_ADMINISTRADOR;
$_SESSION['MEMO_PERFIL_PREPOSTO']                   = PERFIL_PREPOSTO;

?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom.min.js"></script>



<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script type="text/javascript" src="./js/fabrica.js"></script>

<div id="formularioListaMemorandos">
	<?php 
	if( !in_array( PERFIL_FISCAL_CONTRATO, $perfis )  && 
            !in_array( PERFIL_ESPECIALISTA_SQUADRA, $perfis ) &&
            !in_array( PERFIL_SUPER_USUARIO, $perfis ) &&
            !in_array( PERFIL_PREPOSTO, $perfis ) &&
			!in_array( PERFIL_ADMINISTRADOR, $perfis )
          ){ 
	?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td> 
                                    Esta funcionalidade s&oacute; poder&aacute; ser acessada por fiscais do contrato, especialista Squadra
                                    , preposta Squadra
                                </td>
			</tr>
			<tr>
				<td align="center"> <input type="button" value="Voltar" onclick="history.back(-1);"  /> </td>
			</tr>
		</table>
		
	<?php 
	} else {
	?>
	<form id="formListarMemorando" name="formulario" method="post" action="?modulo=sistema/geral/memorando/listar&acao=A ">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<!-- tr>
				<td class="SubtituloDireita" width="25%">Ano de Criação:</td>
				<td>
					<?php 
						
						// SQL que retorna o ano do memorando para a combo anomemorando
						$sql = "select distinct to_char(memodata, 'yyyy') as codigo, to_char(memodata, 'yyyy') as descricao from fabrica.memorando order by to_char(memodata, 'yyyy')";
						echo $db->monta_combo("anoMemorando", $sql, 'S', 'Todos', '', '', '', '80', 'N', 'anoMemorando', '', '', '', '');
					
					?>
				</td>
			</tr>-->
			
			<tr>
				<td class="SubtituloDireita" width="25%" rowspan="3">Memorandos:</td>
				<td>
					<input type="radio" name="tipoMemorando" value="" <?php if( empty($tipoMemorando) ) echo 'checked="checked"';?> >Todos
				</td>
			</tr>
			<tr>
				<td>
					<input type="radio" name="tipoMemorando" value="<?php echo StatusMemorando::MEMORANDO_IMPRESSO?>" <?php if($tipoMemorando == StatusMemorando::MEMORANDO_IMPRESSO) echo 'checked="checked"';?>>Emitido(s)
				</td>
			</tr>
			<tr>
				<td>
					<input type="radio" name="tipoMemorando" value="<?php echo StatusMemorando::MEMORANDO_NAO_IMPRESSO?>" <?php if($tipoMemorando == StatusMemorando::MEMORANDO_NAO_IMPRESSO) echo 'checked="checked"';?>>Não Emitido(s)
				</td>
			</tr>

			<tr>
				<td class="SubtituloDireita" width="25%" rowspan="3">Tipo de Despesa:</td>
				<td>
					<input type="radio" name="tipoDespesa" value="" <?php if( empty($tipoDespesa) ) echo 'checked="checked"';?> >Todos
				</td>
			</tr>
			<?php 
			foreach( $despesaRepositorio->listarAtivos() as $objDespesas ){
				?>
				<tr>
					<td>
						<input type="radio" name="tipoDespesa" value="<?php echo( $objDespesas->getId() ); ?>" <?php if($tipoDespesa == $objDespesas->getId() ) echo 'checked="checked"';?> /> <?php echo( $objDespesas->getDescricao() ); ?>
					</td>
				</tr>
				<?php			
			}
			?>			
			<tr>
				<td class="SubtituloDireita" width="25%"></td>
				<td>
					<input id="botaoPesquisarMemorando" type="submit" value="Pesquisa"/>
                    <?php if(  in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array(PERFIL_FISCAL_CONTRATO, $perfis) ||  ( !in_array( PERFIL_ESPECIALISTA_SQUADRA, $perfis ) && !in_array( PERFIL_PREPOSTO, $perfis ) ) ){ ?>
					<input type="button" value="Novo" onclick="parent.location='?modulo=sistema/geral/memorando/formulario&acao=A'"/>
                    <?php } ?>
					<input type="button" value="Limpar" onclick="parent.location='?modulo=sistema/geral/memorando/listar&acao=A'"/>
				</td>
			</tr>
		</table>
	</form>
</div>

	<?php
	$cabecalho		= array('Ação', 'Status do memorando', 'Empresa','Número do memorando','Servidor responsável','Data do memorando');
	$tamanho		= array("10%","50%","20%","20%","20%");
	$alinhamento	= array("center","center","center","center","center");
	$db->monta_lista( $sqlMemorandos, $cabecalho, 20, 5, 'N', 'center', $par2, "", "", $alinhamento );
	?>

<div id="dialogAjax">
	<img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>
<?php }?>