<?php
$rfdid = $_REQUEST['rfdid'];

$referenciadespesa = new ReferenciaDespesa($rfdid);
extract($referenciadespesa->getDados());

if ($_POST['act'] == 'salvar'){

    if($_REQUEST['rfdid'] == ''){
        $_REQUEST['usucpf']			= $_SESSION['usucpf'];
	    $_REQUEST['rfddtcadastro']	= 'now()';
	    $_REQUEST['rfdstatus']		= 'A';
    }

    $arCampusNulos = array();

    $_REQUEST['rfddtinicio']				= !empty($_REQUEST['rfddtinicio']) ? formata_data_sql( $_REQUEST['rfddtinicio'] ) : null;
    $_REQUEST['rfddttermino']				= !empty($_REQUEST['rfddttermino']) ? formata_data_sql( $_REQUEST['rfddttermino'] ) : null;
    $_REQUEST['rfddtprogramacaoinicio']		= !empty($_REQUEST['rfddtprogramacaoinicio']) ? formata_data_sql( $_REQUEST['rfddtprogramacaoinicio'] ) : null;
    $_REQUEST['rfddtprogramacaotermino']	= !empty($_REQUEST['rfddtprogramacaotermino']) ? formata_data_sql( $_REQUEST['rfddtprogramacaotermino'] ) : null;
    $_REQUEST['rfddtexecucaoinicio']		= !empty($_REQUEST['rfddtexecucaoinicio']) ? formata_data_sql( $_REQUEST['rfddtexecucaoinicio'] ) : null;
    $_REQUEST['rfddtexecucaotermino']		= !empty($_REQUEST['rfddtexecucaotermino']) ? formata_data_sql( $_REQUEST['rfddtexecucaotermino'] ) : null;

    if( empty($_REQUEST['rfddtinicio']) ){
		$arCampusNulos[] = 'rfddtinicio';
    }
    if( empty($_REQUEST['rfddttermino']) ){
		$arCampusNulos[] = 'rfddttermino';
    }
    if( empty($_REQUEST['rfddtprogramacaoinicio']) ){
		$arCampusNulos[] = 'rfddtprogramacaoinicio';
    }
    if( empty($_REQUEST['rfddtprogramacaotermino']) ){
		$arCampusNulos[] = 'rfddtprogramacaotermino';
    }
    if( empty($_REQUEST['rfddtexecucaoinicio']) ){
		$arCampusNulos[] = 'rfddtexecucaoinicio';
    }
    if( empty($_REQUEST['rfddtexecucaotermino']) ){
		$arCampusNulos[] = 'rfddtexecucaotermino';
    }

    $rfdid = $referenciadespesa->popularDadosObjeto()->salvar('','',$arCampusNulos);

    //grava as unidades executoras
    $modelo = new ReferenciaUnidadeExecutora();
    $modelo->deletaItensByRfdid($rfdid);

    if(!empty($_REQUEST['uexid']) && is_array($_REQUEST['uexid'])){
    	foreach ($_REQUEST['uexid'] AS $uexid){
    		if($uexid){
	    		$arDados			= array();
    	        $arDados['uexid']	= $uexid;
        	    $arDados['rfdid']	= $rfdid;

            	$modelo->popularDadosObjeto($arDados)->salvar();
            	$modelo->setDadosNull();
    		}
    	}
    }

    $referenciadespesa->commit();
    $db->sucesso( $_REQUEST['modulo'], '');

}elseif ($_POST['act'] == 'excluir'){
    $dado = array('rfdstatus' => 'I');
    $referenciadespesa->popularDadosObjeto($dado)
          			  ->salvar();

    $referenciadespesa->commit();

    $db->sucesso( $_REQUEST['modulo'], '');
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" rel="stylesheet"></link>
<form method="POST" name="formulario" action="">
    <input type="hidden" name="rfdid" value="<?= $rfdid; ?>"/>
    <input type=hidden   name="act"   value=""/>

    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" width="35%">Nome:</td>
			<td>
				<?= campo_texto("rfdnome", 'S', 'S', '', '20', '20', '', 'N'); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" width="30%">Ano:</td>
			<td>
				<?= campo_texto("rfdano", 'S', 'S', '', '14', '4', '####', 'N'); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" width="30%">Ordem:</td>
			<td>
				<?= campo_texto("rfdordem", 'S', 'S', '', '14', '5', '#####', 'N'); ?>
			</td>
		</tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Início:
            </td>
            <td>
                <?= campo_data2( 'rfddtinicio', 'N', 'S', '', 'S', '','verificaData(this);','','verificaData(this);','','rfddtinicio' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Término:
            </td>
            <td>
                <?= campo_data2( 'rfddttermino', 'N', 'S', '', 'S', '','verificaData(this);','','verificaData(this);','','rfddttermino' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Início Programação:
            </td>
            <td>
                <?= campo_data2( 'rfddtprogramacaoinicio', 'N', 'S', '', 'S', '','verificaData(this);','','verificaData(this);','','rfddtprogramacaoinicio' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Término Programação:
            </td>
            <td>
                <?= campo_data2( 'rfddtprogramacaotermino', 'N', 'S', '', 'S', '','verificaData(this);','','verificaData(this);','','rfddtprogramacaotermino' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Início Execução:
            </td>
            <td>
                <?= campo_data2( 'rfddtexecucaoinicio', 'N', 'S', '', 'S', '','verificaData(this);','','verificaData(this);','','rfddtexecucaoinicio' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Término Execução:
            </td>
            <td>
                <?= campo_data2( 'rfddtexecucaotermino', 'N', 'S', '', 'S', '','verificaData(this);','','verificaData(this);','','rfddtexecucaotermino' ); ?>
            </td>
        </tr>


        <tr>
            <td align='right' class="SubTituloDireita">
                 Unidades Liberadas:
            </td>
            <td>
                <?php
					if(!empty($rfdid)){
						$sql = "SELECT
									*
								FROM
									planointerno.unidadeexecutora_uo
								JOIN
									sisplan.referenciaunidadeexecutora reu ON codigo = reu.uexid
								WHERE reu.rfdid = {$rfdid}";

				        $uexid = $db->carregar($sql);
					}

					$sqlCampos = '';
					if ( usuario_possui_perfil(PERFIL_GESTOR_UNIDADE_DESCENTRALIZADA) || usuario_possui_perfil(PERFIL_TECNICO_UNIDADE_DESCENTRALIZADA) )
					{
						$sqlCampos = "SELECT * FROM planointerno.unidadeexecutora_uo WHERE uexid in ( SELECT ur.uexid FROM sisplan.usuarioresponsabilidade ur WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.uexid is not null AND ur.rpustatus = 'A')";
					}

					if ( usuario_possui_perfil(PERFIL_APOIO_DIRETOR_DEPARTAMENTO) || usuario_possui_perfil(PERFIL_TECNICO_DEPARTAMENTO) || usuario_possui_perfil(PERFIL_DIRETOR_DEPARTAMENTO) )
					{
						$sqlCampos = "SELECT * FROM planointerno.unidadeexecutora_uo WHERE uexid in ( SELECT ur.uexid FROM sisplan.usuarioresponsabilidade ur WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.uexid is not null AND ur.rpustatus = 'A') or uexid = ( SELECT uexid FROM seguranca.usuario where usucpf = '" . $_SESSION['usucpf']."')";
					}

					if ( $db->testa_superuser() ||
						 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) ||
						 usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) ||
						 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) ||
						 usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) ) {
						$sqlCampos = "SELECT * FROM planointerno.unidadeexecutora_uo WHERE TRUE ";
					 }

                    combo_popup( 'uexid', $sqlCampos, 'Selecione as Unidades Executoras', '400x400', 0, array(), '', 'S', true, false, 10, 400, null, null, '', '', null, true, true, '', '' , '', array('descricao'));
                ?>
            </td>
       	</tr>


        <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%"> </td>
        <td>
            <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
            <input type="button" name="novo" value="Novo" onclick="javascript:location.href='?modulo=sistema/tabapoio/referenciadespesa&acao=C'"/>
        </td>
        </tr>
    </table>
</form>
<?php


    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);
    $arCabecalho = array("Ações", "Ano","Nome", "Ordem", "Data Início", "Data Término","Data Início Programação", "Data Término Programação","Data Início Execucão", "Data Término Execucão");

    $acao = '<center>
                <img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {rfdid} )">
                <img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {rfdid} )">
            </center>';

    $rs = $referenciadespesa->getByWhere();

    $oLista = new Lista($arConfig);
    $oLista->setCabecalho( $arCabecalho );
    $oLista->setCorpo( $rs );
    $oLista->setAcao( $acao );
    $oLista->show();
?>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function validar_cadastro(cod){

	selectAllOptions( formulario.uexid );

	if (!validaBranco(document.formulario.rfdano, 'Ano')) return false;
	if (!validaBranco(document.formulario.rfdnome, 'Nome')) return false;
	if (!validaBranco(document.formulario.rfdordem, 'Ordem')) return false;

    document.formulario.act.value = 'salvar';
    document.formulario.submit();
}

function editar(cod){
    document.formulario.act.value = 'carregar';
    document.formulario.rfdid.value = cod;

    document.formulario.submit();
}

function excluir(cod){
    if ( confirm('Tem certeza que deseja apagar esta Referência?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.rfdid.value = cod;

        document.formulario.submit();
    }
}
</script>