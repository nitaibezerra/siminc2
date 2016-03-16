<?php

/*** Recupera as informações sobre a atividade ***/
$atividade = (array) atividade_pegar( $_REQUEST['atiid'] );

/** Verifica se o usuário informou anteriormente os dados:
 *           -> Tipo Andamento;
 *           -> Andamento.
 *  Estas informações são cadastradas na tela de 'Informações Gerais' 
 */
if( !verificaCadastroAndamentoAtividade($atividade['atiid']) )
{
	echo '<script>
			alert("Para acessar esta tela as informações \'Tipo Andamento\' e \'Andamento\' da aba\n\'Informações Gerais\' devem estar corretamente cadastradas.");
			window.location.href = "projetos.php?modulo=principal/atividade_estrategico/atividade&acao=A&atiid='.$atividade['atiid'].'";
		  </script>';
	die;
}

/*** Recebe os dados do formulário ***/
if( $_REQUEST["submetido"] )
{
	if( $_REQUEST["tipo"] == 'excluir' )
	{
		$sql = "DELETE FROM pde.metaatividade WHERE mtaid = ".$_REQUEST["mtaid"];
	}
	else
	{
		/*** Aplica a função 'trim' em todos os emails informados ***/
		foreach($_REQUEST["mtaemails"] as $key=>$value)
		{
			$_REQUEST["mtaemails"][$key] = trim($value);
		}
		/*** Monta a string com os emails a serem gravados no banco ***/
		$emails = implode(",", $_REQUEST["mtaemails"]);
		/*** Prepara a data para ser incluída no banco ***/
		$data	= formata_data_sql($_REQUEST["mtadata"]);
		
		if( $_REQUEST["tipo"] == 'alterar' )
		{
			$sql = "UPDATE
						pde.metaatividade
					SET
						mtametaparcial = ".$_REQUEST["mtametaparcial"].",
						mtaemails = '".$emails."',
						mtadata = '".$data."'
					WHERE
						mtaid = ".$_REQUEST["mtaid"];
			
			unset($_REQUEST["mtaid"]);
			unset($_REQUEST["tipo"]);
		}
		else
		{
			/*** Monta o SQL para inserção no banco ***/
			$sql = "INSERT INTO
						pde.metaatividade(mtametaparcial,mtaemails,mtadata,atiid)
					VALUES
						(".$_REQUEST["mtametaparcial"].", '".$emails."', '".$data."', ".$_REQUEST["atiid"].")";
		}
	}
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Dados gravados com sucesso.');
		  </script>";
}

/*** Cabeçalho ***/
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid'] );
montar_titulo_projeto( $atividade['atidescricao'] );

/**** Recupera os dados gravados ***/
if( $_REQUEST["mtaid"] )
{
	$sql   = "SELECT * FROM pde.metaatividade WHERE mtaid = ".$_REQUEST["mtaid"];
	$dados = $db->carregar($sql);
	if($dados)
	{
		extract( $dados[0] );
	}
}

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>

<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<form method="POST" id="formMeta"> 
			<input type="hidden" id="submetido" name="submetido" value="1" />
			<input type="hidden" name="atiid" value="<?=$atividade['atiid']?>" />
			<input type="hidden" name="tipo" id="tipo" value="<?= (($_REQUEST["tipo"]=='alterar') ? 'alterar' : '')?>" />
			<input type="hidden" name="mtaid" id="mtaid" value="<?= (($_REQUEST["tipo"]=='alterar') ? $_REQUEST["mtaid"] : '')?>" />
			
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Data:</td>
					<td><?=campo_data2('mtadata', 'S', 'S', 'Data', 'S', '', '', $mtadata)?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Meta Parcial:</td>
					<td>
					<?
						/*** Recupera o tipo de andamento da atividade ***/
						$sql 				= "SELECT atitipoandamento FROM pde.atividade WHERE atiid = " . $atividade['atiid'];
						$atitipoandamento	= $db->pegaUm($sql);
						/*** Se for percentual ***/
						if( $atitipoandamento == 'p' )
						{
							/*** Monta a combo com as porcentagens ***/
							echo '<select name="mtametaparcial" id="mtametaparcial" class="CampoEstilo">';
							for($i=0; $i<=100; $i+=10)
							{
								$selected = ( (int)$mtametaparcial == $i ) ? 'selected="selected"' : '';
								
								echo '<option value="'.$i.'" '.$selected.'>'.$i.'%</option>';
							}
							echo "</select>";
						}
						/*** Se for quantitativo ***/
						if( $atitipoandamento == 'q' )
						{
							/*** Monta um campo input text para inserção do valor ***/
							echo '<input type="text" name="mtametaparcial" id="mtametaparcial" value="'.$mtametaparcial.'" class="CampoEstilo" onkeyup="this.value=mascaraglobal(\'##########\',this.value);" />';
						}
					?>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Emails de alerta:</td>
					<td>
						<input type="text" id="email" class="campoEstilo" style="width:200px;" />
						&nbsp;
						<input type="button" value="Incluir Email" onclick="incluirEmail();" />
						<br /><br />
						<table class="listagem" id="tabela_email">
							<tr>
								<th style="text-align:center; width:50px;">Excluir</th>
								<th style="text-align:center; width:150px;">Email</th>
							</tr>
							<? 
							if($mtaemails)
							{
								$mtaemails = explode(',', $mtaemails);
								for($i=0; $i<count($mtaemails); $i++)
								{
									echo '<tr>
											<td style="text-align:center;">
												<img src="../imagens/excluir.gif" style="cursor:pointer;" onclick="excluirEmail(this.parentNode.parentNode.rowIndex);" />
								  				<input type="hidden" name="mtaemails[]" value="'.$mtaemails[$i].'" />
											</td>
											<td style="text-align:center;">
												'.$mtaemails[$i].'
											</td>
										  </tr>';
								}
							}
							?>
						</table>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;"></td>
					<td align='right' class="SubTituloEsquerda">
						<input type="button" value="Salvar" id="btSalvar" onclick="salvarMeta();" />
					</td>
				</tr>
			</table>
			</form>
		</td>
	</tr>
	<tr>
		<td>
	<?
			$sql = "SELECT
						'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarMeta(' || mtaid || ');\" />
					     <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirMeta(' || mtaid || ');\" />' as acoes,
						mta.mtametaparcial || (CASE WHEN ati.atitipoandamento = 'p'	THEN '%' ELSE '' END) as meta,
						mta.mtaemails,
						to_char(mta.mtadata,'dd/mm/yyyy') as data
					FROM
						pde.metaatividade mta
					INNER JOIN
						pde.atividade ati ON ati.atiid = mta.atiid
					WHERE
						mta.atiid = ".$atividade['atiid'];
			
			$cabecalho = array("Alterar/Excluir", "Meta Parcial", "Emails", "Data");
			
			$tamanho		= array( '5%', '25%','60%','10%');
			$alinhamento	= array( 'center', 'center', 'center', 'center');
			
			$db->monta_lista($sql, $cabecalho, 9000, 10, 'S','center', 'S', '', $tamanho, $alinhamento);
			
			?>
			</td>
		</tr>
	</table>

<script type="text/javascript"><!--

/*
 * Realiza as validações dos campos e submete o formulário
 * para gravação no banco de dados.
 */
function salvarMeta()
{
	var form 			= document.getElementById('formMeta');
	var btSalvar		= document.getElementById('btSalvar');
	var mtadata			= document.getElementById('mtadata');
	var mtametaparcial	= document.getElementById('mtametaparcial');
	
	btSalvar.disabled = true;
	
	if( mtadata.value == '' )
	{
		alert("O campo 'Data' deve ser preenchido.");
		mtadata.focus();
		btSalvar.disabled = false;
		return;
	}
	if( mtametaparcial.value == '' )
	{
		alert("O campo 'Meta Parcial' deve ser preenchido.");
		mtametaparcial.focus();
		btSalvar.disabled = false;
		return;
	}
    if( mtametaparcial.value > <?= (($atividade["atimetanumerica"]) ? $atividade["atimetanumerica"] : '9999') ?> )
    {
    	alert("O campo 'Meta Parcial' não pode ter um valor maior que 'Meta Numérica' anteriormente cadastrada.");
		mtametaparcial.focus();
		btSalvar.disabled = false;
		return;
    }
	var nLinhas = tabela_email.rows.length;
	if( nLinhas <= 1 )
	{
		alert("Pelo menos 1(um) email deve ser informado no campo 'Emails de alerta'.");
		btSalvar.disabled = false;
		return;
	}
	
	form.submit();
}

/**
 * Inclui o email informado na tabela de 'Emails de alerta'.
 */
function incluirEmail()
{
	/*** Recupera o objeto input do campo de email ***/
	var email = document.getElementById('email');
	/*** Testa se o email é válido ***/
	if( !checkMail(email.value) )
	{
		alert('O email informado não é válido.');
		email.focus();
		return;
	}
	/*** Recupera o objeto da tabela de emails ***/
	var tabela 		= document.getElementById('tabela_email');
	/*** Adiciona um linha nova na tabela ***/
	var linha 		= tabela.insertRow(1);
	/*** Inclui as duas novas células ***/
	var celExcluir 	= linha.insertCell(0);
	var celEmail 	= linha.insertCell(1);
	/*** Seta o style e o conteúdo da célula 'Excluir' ***/
	celExcluir.style.textAlign 	= "center";
	celExcluir.innerHTML		= "<img src='../imagens/excluir.gif' style='cursor:pointer;' onclick='excluirEmail(this.parentNode.parentNode.rowIndex);' />" +
								  "<input type='hidden' name='mtaemails[]' value='"+email.value+"' />";
	/*** Seta o style e o conteúdo da célula 'Email' ***/
	celEmail.style.textAlign 	= "center";
	celEmail.innerHTML			= email.value;
	/*** Limpa o campo texto do email ***/
	email.value = '';
}

function excluirEmail(index)
{
	var tabela = document.getElementById('tabela_email');
	tabela.deleteRow(index);
}

function alterarMeta(id)
{
	var form 	= document.getElementById('formMeta');
	var tipo  	= document.getElementById('tipo');
	var mtaid	= document.getElementById('mtaid');
	var sub		= document.getElementById('submetido');

	sub.value 	= '0';
	tipo.value  = 'alterar';
	mtaid.value = id;
	form.submit();
}

function excluirMeta(id)
{
	if( confirm('Deseja realmente excluir esta meta?') )
	{
		var form 	= document.getElementById('formMeta');
		var tipo  	= document.getElementById('tipo');
		var mtaid	= document.getElementById('mtaid');
	
		tipo.value  = 'excluir';
		mtaid.value = id;
		form.submit();
	}
}

function checkMail(mail)
{
    var er = new RegExp(/^[A-Za-z0-9_\-\.]+@[A-Za-z0-9_\-\.]{2,}\.[A-Za-z0-9]{2,}(\.[A-Za-z0-9])?/);
    
    if(typeof(mail) == "string")
    {
    	if(er.test(mail))
        {
            return true;
        }
    }
    else if(typeof(mail) == "object")
    {
    	if(er.test(mail.value))
       	{ 
        	return true; 
        }
    }
    else
	{
    	return false;
    }
}

--></script>