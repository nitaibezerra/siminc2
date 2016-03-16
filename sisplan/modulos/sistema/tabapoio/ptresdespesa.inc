<?php
$ptrid = $_REQUEST['ptrid'];
$ptresdespesa = new PtresDespesa($ptrid);
extract($ptresdespesa->getDados());
$ptridcombo = $ptrid;

if ($_POST['act'] == 'salvar'){

	$retorno = true;
	$mensagem = '';

    if($_REQUEST['ptrid'] == ''){
    	if($ptresdespesa->verificaPK($_REQUEST['ptridcombo']) > 0){
			$retorno  = false;
			$mensagem = "O PTRES selecionado já se encontra vinculado.";
    	}else{
	        $ptrid = $ptresdespesa->inserirRegistro($_REQUEST);
    	}
    }else{
    	if($ptresdespesa->verificaPK($_REQUEST['ptridcombo'],$_REQUEST['ptrid']) > 0){
			$retorno  = false;
			$mensagem = "O PTRES selecionado já se encontra vinculado.";
    	}else{
		    $ptrid = $ptresdespesa->alterarRegistro($_REQUEST);
    	}
    }

    //Se o flag for verdadeiro grava as unidades
    if($retorno){
	    $modelo = new PtresUnidadeExecutora();
	    $modelo->deletaItensByPtrid($ptrid);

	    if(!empty($_REQUEST['uexid']) && is_array($_REQUEST['uexid'])){
	    	foreach ($_REQUEST['uexid'] AS $uexid){
	    		$arDados			= array();
	            $arDados['uexid']	= $uexid;
	            $arDados['ptrid']	= $ptrid;

	            $modelo->popularDadosObjeto($arDados)->salvar();
	            $modelo->setDadosNull();
	    	}
	    }

	    $ptresdespesa->commit();
	    $db->sucesso( $_REQUEST['modulo'], '');

    }else{ //se não for da a mensagem e extrai o request
    	echo "<script> alert('".$mensagem."'); </script>";
    	extract($_REQUEST);
    }

}elseif ($_POST['act'] == 'excluir'){
    if( $ptresdespesa->excluir($_REQUEST['ptrid']) ){
    	$ptresdespesa->commit();
	    $db->sucesso( $_REQUEST['modulo'], '');
    }else{
    	$mensagem = "Não é possível excluir o registro, existem outros registros vinculados a ele.";
    	echo "<script>
    				alert('".$mensagem."');
    				location.href='?modulo=sistema/tabapoio/ptresdespesa&acao=C';
    		  </script>";
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" rel="stylesheet"></link>
<form method="POST" name="formulario" action="">
    <input type="hidden" name="ptrid" value="<?= $ptrid; ?>"/>
    <input type=hidden   name="act"   value=""/>

    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" width="30%">PTRES:</td>
			<td>
				<?
				//047730 – PRG.ACA.UNID.LOCAL     – NOME ACAO
                //PTRES  - funcional programatica - Nome da Ação

				$sql = "SELECT
							ptr.ptrid AS codigo,
							ptr.ptres ||' - '|| ptr.prgcod || '.' || ptr.acacod || '.' || ptr.unicod || '.' || ptr.loccod || ' - ' || aca.acadsc AS descricao
						FROM
							sisplan.ptres ptr
						JOIN
						 	monitora.acao aca ON aca.acacod = ptr.acacod AND
										aca.prgcod = ptr.prgcod AND
										aca.loccod = ptr.loccod AND
										aca.unicod = ptr.unicod AND
										aca.prgano = '".$_SESSION['exercicio']."' AND
						 			             aca.acastatus = 'A' AND
						 			             aca.acasnrap IS FALSE
						WHERE
							ptr.ptrstatus = 'A' AND
							ptr.exercicio = '".$_SESSION['exercicio']."'
						ORDER BY
							2";

				$db->monta_combo("ptridcombo",$sql,'S','Selecione...','','','','400','S','ptridcombo');
				?>
			</td>
		</tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Início da Programação:
            </td>
            <td>
                <?= campo_data2( 'ptddtinicio', 'S', 'S', '', 'S', '','verificaData(this);','','verificaData(this);','','ptddtinicio' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Término da Programação:
            </td>
            <td>
                <?= campo_data2( 'ptddttermino', 'S', 'S', '', 'S', '','verificaData(this);','','verificaData(this);','','ptddttermino' ); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">
                 Unidades Liberadas:
            </td>
            <td>
                <?php
					if(!empty($ptrid)){
						$sql = "SELECT
									*
								FROM
									planointerno.unidadeexecutora_uo
								JOIN
									sisplan.ptresunidadeexecutora ptu ON codigo = ptu.uexid
								WHERE ptu.ptrid = {$ptrid}";

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
                    echo obrigatorio();
                ?>
            </td>
       	</tr>
        <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%"> </td>
        <td>
            <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
            <input type="button" name="novo" value="Novo" onclick="javascript:location.href='?modulo=sistema/tabapoio/ptresdespesa&acao=C'"/>
        </td>
        </tr>
    </table>
</form>
<?php


    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);
    $arCabecalho = array("Ações", "Ano","PTRES", "Funcional", "Data Início", "Data Término","Qtd Unidades Liberadas","Data de Cadastro","Usuário");

    $acao = '<center>
                <img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {ptrid} )">
                <img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {ptrid} )">
            </center>';

    $rs = $ptresdespesa->getByWhere();

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

	if (!validaBranco(document.formulario.ptridcombo, 'PTRES')) return false;
	if (!validaBranco(document.formulario.ptddtinicio, 'Data de Início da Programação')) return false;
	if (!validaBranco(document.formulario.ptddttermino, 'Data de Término da Programação')) return false;
	if (!validaBranco(document.getElementById('uexid'), 'Unidades Liberadas')) return;


    document.formulario.act.value = 'salvar';

    document.formulario.submit();
}

function editar(cod){
    document.formulario.act.value = 'carregar';
    document.formulario.ptrid.value = cod;

    document.formulario.submit();
}

function excluir(cod){
    if ( confirm('Tem certeza que deseja apagar este PTRES?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.ptrid.value = cod;

        document.formulario.submit();
    }
}
</script>