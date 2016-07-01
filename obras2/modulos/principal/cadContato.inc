<?php

$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
if (!in_array($_SESSION['obras2']['orgid'], $arOrgid)) {
    $_SESSION['obras2']['orgid'] = '';
}

$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
//$orgid 						 = $_SESSION['obras2']['orgid'];

$contato = new Contato( $_REQUEST['cntid'] );

switch ( $_REQUEST['requisicao'] ){
	case 'salvar':
		$arDado = $_POST;
		
		$arCamposNulo = array();
		if ( empty($arDado['orgid']) ){
			$arDado['orgid'] = null;
			$arCamposNulo[]  = 'orgid'; 	
		}
		if ( empty($arDado['tooid']) ){
			$arDado['tooid'] = null;
			$arCamposNulo[]  = 'tooid'; 	
		}
		
		$contato->popularDadosObjeto( $arDado )
				->salvar(true, true, $arCamposNulo);
		$db->commit();				  		   

		die("<script>
				alert('Operação realizada com sucesso!'); 
				window.location = '?modulo=principal/cadContato&acao=A';
			 </script>");
				  
		break;
	case 'apagar':
		$contato->cntstatus = 'I';
		$contato->salvar();
		$db->commit();				  		   
		die("<script>
				alert('Operação realizada com sucesso!'); 
				window.location = '?modulo=principal/cadContato&acao=A';
			 </script>");
}

extract( $contato->getDados() );

$orgid 						 = $_SESSION['obras2']['orgid'];

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listaContato');
echo montarAbasArray($arAba, "?modulo=principal/cadContato&acao=A&orgid=" . $orgid);

monta_titulo($titulo_modulo, '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="post" id="formulario" name="formulario">
<input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
<input type="hidden" name="cntid" id="cntid" value="<?php echo $cntid ?>"/>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td width="265" class="subtitulodireita">Órgão:</td>
		<td>
			<?php 
			$orgao = new Orgao();
			echo $orgao->pegaDescricao( $orgid );
			?>
			<input type="hidden" name="orgid" id="orgid" value="<?php echo $orgid; ?>">
		</td>
	</tr>
	<tr id="">
		<td class="subtitulodireita">Nome:</td>
		<td>
			<?php
				if ( $usucpf ){
					$sql = "SELECT 
									u.usucpf AS codigo,
									u.usucpf AS value,
									'(' || REPLACE(TO_CHAR(TRIM(u.usucpf)::numeric, '000:000:000-00'),':', '.') || ') ' || u.usunome AS descricao
								FROM
									seguranca.usuario u
								WHERE
									usucpf = '{$usucpf}'";
					$usucpf = $db->pegaLinha( $sql );
				}
				$sql = 	"SELECT 
							usucpf AS codigo, 
							usunome AS descricao
						FROM 
							seguranca.usuario
						WHERE
							1=1
						ORDER BY usunome
						LIMIT 100";
				campo_popup('usucpf',$sql,'Selecione','','400x400','50', 
								array(
									array("codigo"    => "usucpf", 
										  "descricao" => "CPF",
										  "numeric" => "0"),
									array("codigo" 	  => "usunome",
										  "descricao" => "Nome",
										  "numeric" => "0")),
									1
								);
			?>
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">UF:</td>
		<td>
			<?php 
				$estado = new Estado();
				$db->monta_combo('estuf', $estado->listaCombo(), 'S', "Selecione...", '', '', '', '', 'S', 'estuf'); 
			?>
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Fonte:</td>
		<td>
			<?php 
				$tipoOrigemObra = new TipoOrigemObra();
				$db->monta_combo('tooid', $tipoOrigemObra->listaCombo(), 'S', "Selecione...", '', '', '', '', 'N', 'tooid'); 
			?>
		</td>
	</tr>	
	<tr bgcolor="#DEDEDE">
		<td>
			&nbsp;
		</td>
		<td>
			<input name="btn_salvar" value="Salvar" type="button" onclick="enviaFormulario();">
			<input name="btn_novo" value="Novo" type="button" onclick="location.href='?modulo=principal/cadContato&acao=A';">
		</td>
	</tr>	
	<tr>
		<td width="265" class="subtitulodireita" colspan="2">
		&nbsp;
		</td>
	</tr>	
	<tr>
		<td width="265" colspan="2" align="center" style="background-color: #DCDCDC;">
			<b>Lista de Contatos</b>
		</td>
	</tr>	
	<tr>
		<td width="265" class="subtitulodireita" colspan="2" style="background-color: #FFFFFF; ">
			<div style="height: 250px; overflow: auto; text-align: left">
			<?php
			$contato = new Contato();
			$_POST['orgid'] = $orgid;
			$sql	 = $contato->listaSql( $_POST );
			
			$cabecalho = array("Ação", "Nome", "Órgão", "Estado", "Fonte");
			$db->monta_lista($sql,$cabecalho,100,5,'N','center','');
			?>
			</div>
		</td>
	</tr>	
</table>
</form>
<script type="text/javascript">
function alterarCont( cntid ){
	location.href = '?modulo=principal/cadContato&acao=A&cntid=' + cntid;
}

function excluirCont( cntid ){
	if ( confirm('Deseja apagar este contato?') ){
		$('#cntid').val(cntid);
		$('#requisicao').val('apagar');
		$('#formulario').submit();
	}
}

function enviaFormulario(){
	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;

	if ( jQuery('#usucpf').val() == '' ){
		mensagem += 'Nome \n';
		validacao = false;
	}	
	
	if ( jQuery('#estuf').val() == '' ){
		mensagem += 'UF \n';
		validacao = false;
	}	
	
	if (!validacao){
		alert(mensagem);
	}else{
		jQuery('#formulario').submit();
	}
}

</script>
