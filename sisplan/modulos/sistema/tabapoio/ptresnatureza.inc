<?php
$ptnid = $_REQUEST['ptnid'];

$ptresnatureza = new PtresNatureza($ptnid);
extract($ptresnatureza->getDados());

	if($ndpid){
		$sql = " SELECT
					nd.ndpcod AS ndpcod
				 FROM
				 	public.naturezadespesa nd
				WHERE
					nd.ndpid = '". $ndpid ."' ";

		$ndpcod = $db->pegaUm($sql);
	}

	if($_POST['act'] != 'salvar'){
		$ptnvlrlimite = formata_valor($ptnvlrlimite);
	}

if ($_POST['act'] == 'salvar'){

	$sql = "SELECT ndpid FROM public.naturezadespesa WHERE ndpcod = '".$_REQUEST['ndpcod']."' AND ndpsubelementostatus = 'A'";
	$ndpid = $db->pegaUm($sql);

	if($ndpid){
		    if($_REQUEST['ptnid'] == ''){
		        $_REQUEST['usucpf']    = $_SESSION['usucpf'];
		        $_REQUEST['ptnstatus'] = 'A';
		    }

	        $_REQUEST['ndpid']    		= $ndpid;
	        $_REQUEST['ptnvlrlimite']   = desformata_valor( $_REQUEST['ptnvlrlimite'] );

		    $ptnid = $ptresnatureza->popularDadosObjeto()->salvar();
		    $ptresnatureza->commit();
		    $db->sucesso( $_REQUEST['modulo'], '');
	}else{
		echo "<script> alert('Não existe uma natureza da despesa com o código especificado.');</script>";
		extract($_REQUEST);
	}


}elseif ($_POST['act'] == 'excluir'){
    $dado = array('ptnstatus' => 'I');
    $ptresnatureza->popularDadosObjeto($dado)
          ->salvar();
    $ptresnatureza->commit();

    $db->sucesso( $_REQUEST['modulo'], '');
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>
<form method="POST" name="formulario" action="">
    <input type="hidden" name="ptnid" value="<?= $ptnid; ?>"/>
    <input type=hidden   name="act"   value=""/>

    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" width="30%">PTRES:</td>
			<td>
				<?
				$sql = "SELECT
							ptd.ptrid AS codigo,
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
						JOIN
							sisplan.ptresdespesa ptd ON ptd.ptrid = ptr.ptrid AND
														ptd.ptdstatus = 'A'
						WHERE
							ptr.ptrstatus = 'A' AND
							ptr.exercicio = '".$_SESSION['exercicio']."'
						ORDER BY
							2";

				$db->monta_combo("ptrid",$sql,'S','','','','','400','S','ptrid');
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" width="30%">Natureza da despesa:</td>
			<td>
				<?
					$sql_financeiro_natureza = "SELECT
													ndpcod AS codigo,
													ndpdsc AS descricao
												FROM
													naturezadespesa
												WHERE
													ndpsubelementostatus = 'A'
												ORDER BY
													ndpcod";


					texto_popup( 'ndpcod', $sql_financeiro_natureza, 'Natureza da Despesa', 8, 14, '########', '', '' );
					echo obrigatorio();
				?>
				<a style="cursor: pointer;" onclick="javascript: window.location='?modulo=sistema/tabapoio/naturezaDespesa&acao=A';" title="Clique para incluir uma nova Natureza da Despesa">
					<img src="../imagens/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir Natureza da Despesa</b>
				</a>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" width="30%">Valor Limite:</td>
			<td>
				<?php
					echo campo_texto( "ptnvlrlimite", 'N', 'S', '', 15, 15, '###.###.###.###,##', '', 'left', '', 0, "id='ptnvlrlimite'");
					?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" width="30%">Autorizar lançamentos acima do limite estabelecido:</td>
			<td>
				<input type="radio"  name="ptnflglancamento" id="ptnflglancamento_s" value="true" <?= $ptnflglancamento == 't' || $ptnflglancamento == '' ? 'checked="checked"' : ""; ?>  /> <label for="ptnflglancamento_s">Sim</label>
				<input type="radio"  name="ptnflglancamento" id="ptnflglancamento_n" value="false" <?= $ptnflglancamento == 'f' ? 'checked="checked"' : ""; ?>/> <label for="ptnflglancamento_n">Não</label>
			</td>
		</tr>
        <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%"> </td>
        <td>
            <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
            <input type="button" name="novo" value="Novo" onclick="javascript:location.href='?modulo=sistema/tabapoio/ptresnatureza&acao=C'"/>
        </td>
        </tr>
    </table>
</form>
<?php

    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);
    $arCabecalho = array("Ações", "Ano","PTRES", "Natureza", "Valor", "Extrapolar limite");

    $acao = '<center>
                <img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {ptnid} )">
                <img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {ptnid} )">
            </center>';

    $rs = $ptresnatureza->getByWhere();

    $arConfigCol[0] = array('type' => Lista::TYPESTRING);
    $arConfigCol[1] = array('type' => Lista::TYPESTRING);

    $oLista = new Lista($arConfig);
    $oLista->setCabecalho( $arCabecalho );
    $oLista->setCorpo( $rs, $arConfigCol );
    $oLista->setAcao( $acao );
    $oLista->show();
?>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function validar_cadastro(cod){


	if (!validaBranco(document.formulario.ptrid, 'PTRES')) return false;
	if (!validaBranco(document.formulario.ndpcod, 'Natureza da despesa')) return false;

//	Se o campo autorizar for “não” o campo valor é obrigatório ser maior que zero
	if( $('#ptnflglancamento_n:checked').length > 0 ){

		var valorTemp = replaceAll($('#ptnvlrlimite').val(),'.','');
	    var valorFormatado = new Number (valorTemp.replace(",",".") );

		if( valorFormatado < 1 ){
			alert('O campo de valor limite deve ser maior que zero.');
			return false;
		}
	}

	document.formulario.act.value = 'salvar';
    document.formulario.submit();
}

function replaceAll(string, token, newtoken) {
    while (string.indexOf(token) != -1) {
        string = string.replace(token, newtoken);
    }
    return string;
}

function editar(cod){
    document.formulario.act.value = 'carregar';
    document.formulario.ptnid.value = cod;

    document.formulario.submit();
}

function excluir(cod){
    if ( confirm('Tem certeza que deseja apagar esta Natureza?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.ptnid.value = cod;

        document.formulario.submit();
    }
}
</script>