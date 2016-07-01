<?php

if($_REQUEST['arq']){
	
	$file = "instrutivo_pse.pdf"; 
	header('Cache-Control: public');
	header("Content-type: application/pdf");
	header("Content-Disposition: attachment;filename=instrutivo_pse.pdf");
	$file = $_SERVER["DOCUMENT_ROOT"]."/".$file;
	// mostra o download
	readfile($file);
	
	print "<script>
				location.href='pse.php?modulo=principal/cadastroTermoCompromisso&acao=A';
			</script>";
	exit;
}

if(!$_SESSION['pse']['muncod']){
	?>
	<script>
		alert("É necessário informar um município\n ou sua sessão expirou. Por favor, entre novamente no PSE!");
		location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?
	exit;
}





//verifica portaria
$sql = "select porano from pse.portariapse where pormunicipio = '{$_SESSION['pse']['muncod']}'";
$portariaAno = $db->pegaUm($sql);
$dataAtual = date('Y').date('m').date('d');
$_SESSION['permissaoedicao'] = false;
$_SESSION['mensagem'] = false;

$pflcod = pegaPerfil($_SESSION['usucpf']);
if($pflcod == SUPER_USUARIO){
	$_SESSION['permissaoedicao'] = true;
}


//verifica sessao edição
/*
$sql = "select codigoibge from pse.apoiomunicipio where codigoibge = '{$_SESSION['pse']['muncod']}'";
$codigoibge = $db->pegaUm($sql);
if(($dataAtual >= 20111215 && $dataAtual <= 20120119) && $codigoibge){
	$_SESSION['permissaoedicao'] = true;
}
elseif(($dataAtual >= 20120120 && $dataAtual <= 20120224)){
	if(!$_SESSION['permissaoedicaoSemana']){
		?>
		<script>
			location.href='pse.php?modulo=principal/cadastroTermoSemana&acao=A';
		</script>
		<?
		exit;	
	}
}
*/
/*
if($portariaAno == '2011'){
	//se a data do servidor for maior do que 30/09/2011, não disponibilizar o
    //sistema para edição. Somente para consulta e impressão do Termo de Compromisso.
	if($dataAtual > 20110930 && !$db->testa_superuser()) $_SESSION['permissaoedicao'] = false;
}else{
	//se o ano da portaria do usuário não for igual a 2011 e a data do
    //Servidor for menor do que 01/10/2011, não permitir acesso do usuário
    //ao sistema e mostrar uma mensagem no vídeo.
	if($dataAtual < 20110930 && !$db->testa_superuser()) $_SESSION['mensagem'] = true;
}



//verifica perfil
$usucpf = $_SESSION['usucpf'];
$pflcod = pegaPerfil($usucpf);
if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == CONSULTA || $pflcod == PARCEIRO){
	$_SESSION['mensagem'] = true;
}

$pflcod = pegaPerfil($_SESSION['usucpf']);
if($pflcod == SECRETARIA_ESTADUAL && $_SESSION['pse']['muncod'] != '5300108'){
	$_SESSION['permissaoedicao'] = false;
}
*/






function inserirDados(){
	global $db;
	
	$sql = "INSERT INTO pse.contratualizacao
			(
            	entid,
            	conano, 
            	conseendereco,
            	conseddd, 
            	consetelefone, 
            	conseemailsecretaria, 
	            consenomesecretario, 
	            consecpf, 
	            conseemailsecretario, 
	            conssendereco,
	            conssddd, 
	            consstelefone, 
	            conssemailsecretaria, 
	            conssnomesecretario, 
	            consscpf, 
	            conssemailsecretario, 
	            congtisenome, 
	            congtisecpf, 
	            congtisecargofuncao,
	            congtiseddd, 
	            congtisetelefone, 
	            congtiseemail, 
	            congtissnome, 
	            congtisscpf, 
	            congtisscargofuncao,
	            congtissddd, 
	            congtisstelefone, 
	            congtissemail, 
	            consecnpj, 
	            conseidentidade, 
	            conseorgaoexp, 
	            conseestadocivil, 
	            consscnpj, 
	            conssidentidade, 
	            conssorgaoexp, 
	            conssestadocivil,
	            conesfatuarapse,
				concoberturacomp1,
				conpercentualcomp1,
				concoberturacomp2,
				conpercentualcomp2
	        ) VALUES (
	        	'".$_POST['entid']."',
	        	'".$_POST['conano']."',
	        	'".$_POST['conseendereco']."',
	        	'".$_POST['conseddd']."',
	        	'".$_POST['consetelefone']."',
	        	'".$_POST['conseemailsecretaria']."',
	        	'".$_POST['consenomesecretario']."',
	        	'".str_replace('-','',str_replace('.','',$_POST['consecpf']))."',
	        	'".$_POST['conseemailsecretario']."',
	        	'".$_POST['conssendereco']."',
	        	'".$_POST['conssddd']."',
	        	'".$_POST['consstelefone']."',
	        	'".$_POST['conssemailsecretaria']."',
	        	'".$_POST['conssnomesecretario']."',
	        	'".str_replace('-','',str_replace('.','',$_POST['consscpf']))."',
	        	'".$_POST['conssemailsecretario']."',
	        	'".$_POST['congtisenome']."',
	        	'".str_replace('-','',str_replace('.','',$_POST['congtisecpf']))."',
	        	'".$_POST['congtisecargofuncao']."',
	        	'".$_POST['congtiseddd']."',
	        	'".$_POST['congtisetelefone']."',
	        	'".$_POST['congtiseemail']."',
	        	'".$_POST['congtissnome']."',
	        	'".str_replace('-','',str_replace('.','',$_POST['congtisscpf']))."',
	        	'".$_POST['congtisscargofuncao']."',
	        	'".$_POST['congtissddd']."',
	        	'".$_POST['congtisstelefone']."',
	        	'".$_POST['congtissemail']."',
	        	'".str_replace('.','',str_replace('/','',str_replace('-','',$_POST['consecnpj'])))."',
	        	'".$_POST['conseidentidade']."',
	        	'".$_POST['conseorgaoexp']."',
	        	'".$_POST['conseestadocivil']."',
	        	'".str_replace('.','',str_replace('/','',str_replace('-','',$_POST['consscnpj'])))."',
	        	'".$_POST['conssidentidade']."',
	        	'".$_POST['conssorgaoexp']."',
	        	'".$_POST['conssestadocivil']."',
	        	0,0,0,0,0
	        ) returning conid";
	
	
	$conid = $db->executar($sql);
	$db->commit();
}



function alterarDados(){
	global $db;
	
	
	$sql = "UPDATE 
				pse.contratualizacao
			SET 
	            entid = '".$_POST['entid']."',
				conano = '".$_POST['conano']."',
				conseendereco = '".$_POST['conseendereco']."',
				conseddd = '".$_POST['conseddd']."',
				consetelefone = '".$_POST['consetelefone']."',
				conseemailsecretaria = '".$_POST['conseemailsecretaria']."',
				consenomesecretario = '".$_POST['consenomesecretario']."',
				consecpf = '".str_replace('-','',str_replace('.','',$_POST['consecpf']))."',
				conseemailsecretario = '".$_POST['conseemailsecretario']."',
				conssendereco = '".$_POST['conssendereco']."',
				conssddd = '".$_POST['conssddd']."',
				consstelefone = '".$_POST['consstelefone']."',
				conssemailsecretaria = '".$_POST['conssemailsecretaria']."',
				conssnomesecretario = '".$_POST['conssnomesecretario']."',
				consscpf = '".str_replace('-','',str_replace('.','',$_POST['consscpf']))."',
				conssemailsecretario = '".$_POST['conssemailsecretario']."',
				congtisenome = '".$_POST['congtisenome']."',
				congtisecpf = '".str_replace('-','',str_replace('.','',$_POST['congtisecpf']))."',
				congtisecargofuncao = '".$_POST['congtisecargofuncao']."',
				congtiseddd = '".$_POST['congtiseddd']."',
				congtisetelefone = '".$_POST['congtisetelefone']."',
				congtiseemail = '".$_POST['congtiseemail']."',
				congtissnome = '".$_POST['congtissnome']."',
				congtisscpf = '".str_replace('-','',str_replace('.','',$_POST['congtisscpf']))."',
				congtisscargofuncao = '".$_POST['congtisscargofuncao']."',
				congtissddd = '".$_POST['congtissddd']."',
				congtisstelefone = '".$_POST['congtisstelefone']."',
				congtissemail = '".$_POST['congtissemail']."',
				consecnpj = '".str_replace('.','',str_replace('/','',str_replace('-','',$_POST['consecnpj'])))."',
				conseidentidade = '".$_POST['conseidentidade']."',
				conseorgaoexp = '".$_POST['conseorgaoexp']."',
				conseestadocivil = '".$_POST['conseestadocivil']."',
				consscnpj = '".str_replace('.','',str_replace('/','',str_replace('-','',$_POST['consscnpj'])))."',
				conssidentidade = '".$_POST['conssidentidade']."',
				conssorgaoexp = '".$_POST['conssorgaoexp']."',
				conssestadocivil = '".$_POST['conssestadocivil']."'
			WHERE 
				conid = ".$_POST['conid'];

	$db->executar($sql);					
	$db->commit();
}






if($_POST){
	
	if($_POST['entid']){
		if(!$_POST['conid']) {
			inserirDados();
		}
		else{
			alterarDados();
		}
		unset($_POST);
		print "<script>
				alert('Operação realizada com sucesso!');
				location.href='pse.php?modulo=principal/cadastroTermoCompromisso&acao=A';
			</script>";
		
	}
	else{
		print "<script>
					alert('Erro ao gravar os dados! Favor entrar em contato com o administrador do sistema.');
					location.href='pse.php?modulo=principal/cadastroTermoCompromisso&acao=A';
			  </script>";
	}
	//header( "Location: demandas.php?modulo=sistema/apoio/origemDemanda&acao=A" );
	exit();
	
}



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

if($_SESSION['mensagem'] == true){
	?>
		<br>
		<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" >
			<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">PSE - Termo de Compromisso</label></td></tr>
			<tr><td bgcolor="#e9e9e9" align="center">&nbsp;<br>
						<table width="50%">
							<tr>
								<td>
									<b>Atenção  Senhore(a)s gestore(a)s!</b>
									<br><br>O Sistema de Monitoramento do PSE está sendo atualizado de acordo com o novo modelo de gestão  intersetorial do Programa que considera a necessidade de pactuações das metas das ações de prevenção, promoção e atenção à saúde a serem implantadas/implementadas pelos Municípios, de modo a possibilitar as ações de saúde nas escolas. 
									<br><br>Assim:
									<br>Fica definido na forma do <b><u>Anexo II</u> da PORTARIA INTERMINISTERIAL Nº 1.910, DE 8 DE AGOSTO DE 2011</b>, os Municípios aptos a assinarem o Termo de Compromisso Municipal do Programa Saúde na Escola (PSE), exercício 2011/2012, que deverá ser realizada a contar da data de publicação desta Portaria até o <b>dia 30 de setembro de 2011</b>.
									<br><br><b>Para municípios que aderiram ao PSE em portarias anteriores</b> <br> Fica definido no <b><u>Anexo III</u> da PORTARIA INTERMINISTERIAL Nº 1.910, DE 8 DE AGOSTO DE 2011</b>, os Municípios aptos a assinarem o Termo de Compromisso Municipal do PSE, exercício 2011/2012, que deverá ser realizada no período do dia 30 de setembro a 30 de novembro de 2011.
									<br><br>
								</td>
							</tr>
						</table>
				</td>
			</tr>
		</table>		
	<?php 
}
else{


		$sql = "SELECT mundescricao as municipio, estuf as uf
				FROM territorios.municipio
				WHERE muncod = '".$_SESSION['pse']['muncod']."'";
		$mun = $db->pegaLinha($sql);
		
		
		//pega entid da SECRETARIA MUNICIPAL DE EDUCAÇÃO de um determinado municipio
		/*
		$sql = "select u.entid, e.entnome 
				from seguranca.usuario u
				left join entidade.entidade e on e.entid = u.entid
				where u.usucpf='".$_SESSION['usucpf']."'";
		*/
		$sql = "select et.entid, et.entnome from entidade.entidade et
				inner join entidade.endereco ed on ed.entid=et.entid
				inner join entidade.funcaoentidade fe on  fe.entid = et.entid AND fe.funid = 7
				where fe.fuestatus='A' and muncod = '".$_SESSION['pse']['muncod']."'
				order by 1 limit 1";
		$secretaria = $db->pegaLinha($sql);
		
		
		$entid = $secretaria['entid'];
		$conano = $_SESSION["exercicio"];
		
		
		
		$titulo = "<span style='margin-left: 150px'>PSE - Termo de Compromisso - <a href='javascript:void(0);' onclick='abreArquivoPse();'><b><font color=red>Clique para baixar o instrutivo PSE</font></b></a></span>";
		$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.<br>
		<table width="100%" align="center"  cellpadding="2" cellspacing="1">
		<tr>
			<td width="10%" style="text-align: right;" class="SubTituloDireita">UF:</td>
			<td width="90%" class="SubTituloEsquerda">'.$mun['uf'].'</td>
		</tr>
		<tr>
			<td style="text-align: right;" class="SubTituloDireita">MUNICÍPIO:</td>
			<td class="SubTituloEsquerda">'.$mun['municipio'].'</td>
		</tr>
		<tr>
			<td style="text-align: right;" class="SubTituloDireita">SECRETARIA:</td>
			<td class="SubTituloEsquerda">'.$secretaria['entnome'].'</td>
		</tr>
		</table>';

		echo "<br>";
		
		/*
		$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoCompromisso&acao=A"),
					  1 => array("id" => 2, "descricao" => "Monitoramento de Ações", 	"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramento&acao=A"),
					  2 => array("id" => 3, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
				  	  );
		
		echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
		*/
		
		monta_titulo( $titulo, $dsctitulo );
		echo "<br>";
		
		
		$db->cria_aba( $abacod_tela, $url, '');
		
		
		if($entid){
			$sql = "SELECT  conid,
							entid,
			            	conano, 
			            	conseendereco,
			            	conseddd, 
			            	consetelefone, 
			            	conseemailsecretaria, 
				            consenomesecretario, 
				            consecpf, 
				            conseemailsecretario, 
				            conssendereco,
				            conssddd, 
				            consstelefone, 
				            conssemailsecretaria, 
				            conssnomesecretario, 
				            consscpf, 
				            conssemailsecretario, 
				            congtisenome, 
				            congtisecpf, 
				            congtisecargofuncao,
				            congtiseddd, 
				            congtisetelefone, 
				            congtiseemail, 
				            congtissnome, 
				            congtisscpf, 
				            congtisscargofuncao,
				            congtissddd, 
				            congtisstelefone, 
				            congtissemail, 
				            consecnpj, 
				            conseidentidade, 
				            conseorgaoexp, 
				            conseestadocivil, 
				            consscnpj, 
				            conssidentidade, 
				            conssorgaoexp, 
				            conssestadocivil 
				     FROM 
				     		pse.contratualizacao
				     WHERE 
				     		entid = $entid";
			$dados = $db->carregar($sql);
			if($dados) extract($dados[0]);
			//if($vlpvalorponto) $vlpvalorponto = number_format($vlpvalorponto,2,",",".");
			if($consecpf) $consecpf = formatar_cpf($consecpf);
			if($consscpf) $consscpf = formatar_cpf($consscpf);
			if($congtisecpf) $congtisecpf = formatar_cpf($congtisecpf);
			if($congtisscpf) $congtisscpf = formatar_cpf($congtisscpf);
			
			if($consecnpj) $consecnpj = formatar_cnpj($consecnpj);
			if($consscnpj) $consscnpj = formatar_cnpj($consscnpj);
			
			if(!$_SESSION['conid'] && $conid) $_SESSION['conid'] = $conid;
			
			if($conid){
				$sql = "SELECT arqid FROM pse.contratualizacao WHERE conid = ".$conid;
				$arqid = $db->pegaUm( $sql );
				if( !empty( $arqid ) ) $_SESSION['permissaoedicao'] = false;
			}
		}
		
		
		?>
		
		<form id="formulario" name="formulario" action="" method="post">
		
		<input type="hidden" name="conid" value="<?=$conid?>">
		<input type="hidden" name="entid" value="<?=$entid?>">
		<input type="hidden" name="conano" value="<?=$conano?>">
		
		<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
		<tr>
			<td class="SubTituloEsquerda" colspan='3'>
				<br>
			     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Cadastro das informações das Secretarias Municipais da Saúde e Educação: </b>
			     <br><br>
			</td>
		</tr>
		<tr>
			<td style="background: rgb(238, 238, 238)" colspan='3'>
				<br>
			     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; O modo de responsabilidade do PSE será a partir de assinatura de Termo de Compromisso entre as Secretarias Municipais de Saúde e de Educação, com metas pactuadas de implementação das ações e a transferência dos recursos <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; vinculada ao alcance das metas que serão monitoradas por meio do SIMEC.</b>
			     <br><br>
			</td>
		</tr>
		<tr>
			<td style="background: rgb(238, 238, 238)" colspan='2'>
				<p style="margin-left: 269px;"><b>SECRETARIA DE EDUCAÇÃO</b></p>
			</td>
			<td style="background: rgb(238, 238, 238)"><b>SECRETARIA DE SAÚDE</b></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">Endereço completo:</td>
			<td width="40%"><?=campo_texto( 'conseendereco', 'S', 'S', '', 98, 60, '', '', '', '', 0, 'tabindex=1');?></td>
			<td width="40%"><?=campo_texto( 'conssendereco', 'S', 'S', '', 98, 60, '', '', '', '', 0, 'tabindex=11');?></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">(DDD) Telefone:</td>
			<td width="40%">
							(<?=campo_texto( 'conseddd', 'N', 'S', 'Informe o DDD (Ex: 61)', 1, 2, '##', '', '', '', 0, 'tabindex=2');?>)
							<?=campo_texto( 'consetelefone', 'S', 'S', 'Informe o Telefone (Ex: 9999-9999)', 90, 20, '####-####', '', '', '', 0, 'tabindex=2');?>
			</td>
			<td width="40%">
							(<?=campo_texto( 'conssddd', 'N', 'S', 'Informe o DDD (Ex: 61)', 1, 2, '##', '', '', '', 0, 'tabindex=12');?>)
							<?=campo_texto( 'consstelefone', 'S', 'S', 'Informe o Telefone (Ex: 9999-9999)', 90, 20, '####-####', '', '', '', 0, 'tabindex=12');?>			
			</td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">E-mail da Secretaria:</td>
			<td width="40%"><?=campo_texto( 'conseemailsecretaria', 'S', 'S', '', 98, 70, '', '', '', '', 0, 'tabindex=3');?></td>
			<td width="40%"><?=campo_texto( 'conssemailsecretaria', 'S', 'S', '', 98, 70, '', '', '', '', 0, 'tabindex=13');?></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">CNPJ da Secretaria:</td>
			<td width="40%"><?=campo_texto( 'consecnpj', 'S', 'S', '', 98, 18, '##.###.###/####-##', '', '', '', 0, 'tabindex=4');?></td>
			<td width="40%"><?=campo_texto( 'consscnpj', 'S', 'S', '', 98, 18, '##.###.###/####-##', '', '', '', 0, 'tabindex=14');?></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">Nome do(a) Secretário(a):</td>
			<td width="40%"><?=campo_texto( 'consenomesecretario', 'S', 'S', '', 98, 60, '', '', '', '', 0, 'tabindex=5');?></td>
			<td width="40%"><?=campo_texto( 'conssnomesecretario', 'S', 'S', '', 98, 60, '', '', '', '', 0, 'tabindex=15');?></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">CPF do(a) Secretário(a):</td>
			<td width="40%"><?=campo_texto( 'consecpf', 'S', 'S', '', 98, 14, '###.###.###-##', '', '', '', 0, 'tabindex=6');?></td>
			<td width="40%"><?=campo_texto( 'consscpf', 'S', 'S', '', 98, 14, '###.###.###-##', '', '', '', 0, 'tabindex=16');?></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">Carteira de Identidade:</td>
			<td width="40%" style="background: rgb(238, 238, 238)">
				<?=campo_texto( 'conseidentidade', 'S', 'S', '', 40, 20, '', '', '', '', 0, 'tabindex=7');?>
				<span style="margin-left: 71px;">
					Orgão Expeditor:
					<?=campo_texto( 'conseorgaoexp', 'S', 'S', '', 20, 11, '', '', '', '', 0, 'tabindex=8');?>
				</span>
			</td>
			<td width="40%" style="background: rgb(238, 238, 238)">
				<?=campo_texto( 'conssidentidade', 'S', 'S', '', 40, 20, '', '', '', '', 0, 'tabindex=17');?>
				<span style="margin-left: 71px;">
					Orgão Expeditor:
					<?=campo_texto( 'conssorgaoexp', 'S', 'S', '', 20, 11, '', '', '', '', 0, 'tabindex=18');?> 
				</span>
			</td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">Estado Civil:</td>
			<td width="40%"><?=campo_texto( 'conseestadocivil', 'S', 'S', '', 98, 15, '', '', '', '', 0, 'tabindex=9');?></td>
			<td width="40%"><?=campo_texto( 'conssestadocivil', 'S', 'S', '', 98, 15, '', '', '', '', 0, 'tabindex=19');?></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">E-mail do(a) Secretário(a):</td>
			<td width="40%"><?=campo_texto( 'conseemailsecretario', 'S', 'S', '', 98, 70, '', '', '', '', 0, 'tabindex=10');?></td>
			<td width="40%"><?=campo_texto( 'conssemailsecretario', 'S', 'S', '', 98, 70, '', '', '', '', 0, 'tabindex=20');?></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" colspan='3'>&nbsp;</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" colspan='3'>
				<br>
			     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Cadastro das informações dos representantes nos GTIs das Secretarias Municipais da Saúde e Educação:</b>
			     <br><br>
			</td>
		</tr>
		<tr>
			<td style="background: rgb(238, 238, 238)" colspan='3'>
				<br>
			     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; É responsabilidade das Secretarias Municipais de Saúde e Educação constituir GTI - Grupo de Trabalho Intersetorial Municipal de planejamento, execução do recurso financeiro do PSE, acompanhamento e monitoramento das ações <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; do PSE no território, com representantes da saúde, educação e de outros órgãos / instituições.</b>
			     <br><br>
			</td>
		</tr>
		<tr>
			<td style="background: rgb(238, 238, 238)" colspan='2'>
				<p style="margin-left: 269px;"><b>SECRETARIA DE EDUCAÇÃO</b></p>
			</td>
			<td style="background: rgb(238, 238, 238)"><b>SECRETÁRIA DE SAÚDE</b></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">Nome Completo:</td>
			<td width="40%"><?=campo_texto( 'congtisenome', 'S', 'S', '', 98, 60, '', '', '', '', 0, 'tabindex=21');?></td>
			<td width="40%"><?=campo_texto( 'congtissnome', 'S', 'S', '', 98, 60, '', '', '', '', 0, 'tabindex=26');?></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">CPF:</td>
			<td width="40%"><?=campo_texto( 'congtisecpf', 'S', 'S', '', 98, 14, '###.###.###-##', '', '', '', 0, 'tabindex=22');?></td>
			<td width="40%"><?=campo_texto( 'congtisscpf', 'S', 'S', '', 98, 14, '###.###.###-##', '', '', '', 0, 'tabindex=27');?></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">Cargo / Função:</td>
			<td width="40%"><?=campo_texto( 'congtisecargofuncao', 'S', 'S', '', 98, 30, '', '', '', '', 0, 'tabindex=23');?></td>
			<td width="40%"><?=campo_texto( 'congtisscargofuncao', 'S', 'S', '', 98, 30, '', '', '', '', 0, 'tabindex=28');?></td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">(DDD) Telefone:</td>
			<td width="40%">
							(<?=campo_texto( 'congtiseddd', 'N', 'S', 'Informe o DDD (Ex: 61)', 1, 2, '##', '', '', '', 0, 'tabindex=24');?>)
							<?=campo_texto( 'congtisetelefone', 'S', 'S', 'Informe o Telefone (Ex: 9999-9999)', 90, 20, '####-####', '', '', '', 0, 'tabindex=24');?>			
			</td>
			<td width="40%">
							(<?=campo_texto( 'congtissddd', 'N', 'S', 'Informe o DDD (Ex: 61)', 1, 2, '##', '', '', '', 0, 'tabindex=29');?>)
							<?=campo_texto( 'congtisstelefone', 'S', 'S', 'Informe o Telefone (Ex: 9999-9999)', 90, 20, '####-####', '', '', '', 0, 'tabindex=29');?>			
			</td>
		</tr>
		<tr>
			<td width="20%" class="SubTituloDireita">E-mail:</td>
			<td width="40%"><?=campo_texto( 'congtiseemail', 'S', 'S', '', 98, 70, '', '', '', '', 0, 'tabindex=25');?></td>
			<td width="40%"><?=campo_texto( 'congtissemail', 'S', 'S', '', 98, 70, '', '', '', '', 0, 'tabindex=30');?></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" colspan='3'>&nbsp;</td>
		</tr>
		<tr>
			<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='3'>
				<input type="button" class="botao" name="btSalvar" id="btSalvar" value="Salvar" onclick="validaForm();">
			</td>
		</tr>
		</table>
		
		</form>
		
		<script type="text/javascript">
		
		var d = document.formulario;
		
		function validaForm(){
		
			/*
			var obj = document.getElementsByTagName("input");
			var total = document.getElementsByTagName("input").length;
			var flagvalida = 0;
		
			for(i=0; i<total; i++){
				if(obj[i].type == 'text' && obj[i].value == ''){
					flagvalida = 1;
					break;
				}
			}
		
			if(flagvalida == 1){
				alert ('Favor Preencher todos os campos obrigatórios.');
				obj[i].focus();
				return false;
			}
			*/
		
			//SECRETARIA DE EDUCAÇÃO
			if( trim(d.conseendereco.value) == '' ){ 
					alert('Campo obrigatório: Endereço Completo da SECRETARIA DE EDUCAÇÃO'); 
					d.conseendereco.focus();
					return false; 
			}
			if( trim(d.consetelefone.value) == '' || trim(d.conseddd.value) == '' ){ 
					alert('Campo obrigatório: (DDD) Telefone da SECRETARIA DE EDUCAÇÃO'); 
					d.consetelefone.focus();
					return false; 
			}
			if( trim(d.conseemailsecretaria.value) == '' ){ 
					alert('Campo obrigatório: E-mail da SECRETARIA DE EDUCAÇÃO'); 
					d.conseemailsecretaria.focus();
					return false; 
			}
			if( trim(d.consecnpj.value) == '' ){ 
					alert('Campo obrigatório: CNPJ da SECRETARIA DE EDUCAÇÃO'); 
					d.consecnpj.focus();
					return false; 
			}
			if( trim(d.consenomesecretario.value) == '' ){ 
					alert('Campo obrigatório: Nome do(a) Secretário(a) da SECRETARIA DE EDUCAÇÃO'); 
					d.consenomesecretario.focus();
					return false; 
			}
			if( trim(d.consecpf.value) == '' ){ 
					alert('Campo obrigatório: CPF do(a) Secretário(a) da SECRETARIA DE EDUCAÇÃO'); 
					d.consecpf.focus();
					return false; 
			}
			if( trim(d.conseidentidade.value) == '' ){ 
					alert('Campo obrigatório: Carteira de Identidade do(a) Secretário(a) da SECRETARIA DE EDUCAÇÃO');
					d.conseidentidade.focus(); 
					return false; 
			}
			if( trim(d.conseorgaoexp.value) == '' ){ 
					alert('Campo obrigatório: Orgão Expeditor da Carteira de Identidade do(a) Secretário(a) da SECRETARIA DE EDUCAÇÃO');
					d.conseorgaoexp.focus(); 
					return false; 
			}
			if( trim(d.conseestadocivil.value) == '' ){ 
					alert('Campo obrigatório: Estado Civil do(a) Secretário(a) da SECRETARIA DE EDUCAÇÃO'); 
					d.conseestadocivil.focus();
					return false; 
			}
			if( trim(d.conseemailsecretario.value) == '' ){ 
					alert('Campo obrigatório: E-mail do(a) Secretário(a) da SECRETARIA DE EDUCAÇÃO'); 
					d.conseemailsecretario.focus();
					return false; 
			}
		
			//SECRETARIA DE SAÚDE
			if( trim(d.conssendereco.value) == '' ){ 
					alert('Campo obrigatório: Endereço Completo da SECRETARIA DE SAÚDE'); 
					d.conssendereco.focus();
					return false; 
			}
			if( trim(d.consstelefone.value) == '' || trim(d.conssddd.value) == '' ){ 
					alert('Campo obrigatório: (DDD) Telefone da SECRETARIA DE SAÚDE'); 
					d.consstelefone.focus();
					return false; 
			}
			if( trim(d.conssemailsecretaria.value) == '' ){ 
					alert('Campo obrigatório: E-mail da SECRETARIA DE SAÚDE'); 
					d.conssemailsecretaria.focus();
					return false; 
			}
			if( trim(d.consscnpj.value) == '' ){ 
					alert('Campo obrigatório: CNPJ da SECRETARIA DE SAÚDE'); 
					d.consscnpj.focus();
					return false; 
			}
			if( trim(d.conssnomesecretario.value) == '' ){ 
					alert('Campo obrigatório: Nome do(a) Secretário(a) da SECRETARIA DE SAÚDE'); 
					d.conssnomesecretario.focus();
					return false; 
			}
			if( trim(d.consscpf.value) == '' ){ 
					alert('Campo obrigatório: CPF do(a) Secretário(a) da SECRETARIA DE SAÚDE'); 
					d.consscpf.focus();
					return false; 
			}
			if( trim(d.conssidentidade.value) == '' ){ 
					alert('Campo obrigatório: Carteira de Identidade do(a) Secretário(a) da SECRETARIA DE SAÚDE');
					d.conssidentidade.focus();
					return false; 
			}
			if( trim(d.conssorgaoexp.value) == '' ){ 
					alert('Campo obrigatório: Orgão Expeditor da Carteira de Identidade do(a) Secretário(a) da SECRETARIA DE SAÚDE');
					d.conssorgaoexp.focus(); 
					return false; 
			}
			if( trim(d.conssestadocivil.value) == '' ){ 
					alert('Campo obrigatório: Estado Civil do(a) Secretário(a) da SECRETARIA DE SAÚDE');
					d.conssestadocivil.focus();
					return false; 
			}
			if( trim(d.conssemailsecretario.value) == '' ){ 
					alert('Campo obrigatório: E-mail do(a) Secretário(a) da SECRETARIA DE SAÚDE');
					d.conssemailsecretario.focus(); 
					return false; 
			}
		
		
			//SECRETARIA DE EDUCAÇÃO 2º parte
			if( trim(d.congtisenome.value) == '' ){ 
					alert('Campo obrigatório: Nome Completo do(a) Representante da SECRETARIA DE EDUCAÇÃO'); 
					d.congtisenome.focus();
					return false; 
			}
			if( trim(d.congtisecpf.value) == '' ){ 
					alert('Campo obrigatório: CPF do(a) Representante da SECRETARIA DE EDUCAÇÃO'); 
					d.congtisecpf.focus();
					return false; 
			}
			if( trim(d.congtisecargofuncao.value) == '' ){ 
					alert('Campo obrigatório: Cargo/Função do(a) Representante da SECRETARIA DE EDUCAÇÃO'); 
					d.congtisecargofuncao.focus();
					return false; 
			}
			if( trim(d.congtisetelefone.value) == '' || trim(d.congtiseddd.value) == '' ){ 
					alert('Campo obrigatório: (DDD) Telefone do(a) Representante da SECRETARIA DE EDUCAÇÃO'); 
					d.congtisetelefone.focus();
					return false; 
			}
			if( trim(d.congtiseemail.value) == '' ){ 
					alert('Campo obrigatório: E-mail do(a) Representante da SECRETARIA DE EDUCAÇÃO'); 
					d.congtiseemail.focus();
					return false; 
			}
			
			
			//SECRETARIA DE SAÚDE 2º parte
			if( trim(d.congtissnome.value) == '' ){ 
					alert('Campo obrigatório: Nome Completo do(a) Representante da SECRETARIA DE SAÚDE'); 
					d.congtissnome.focus();
					return false; 
			}
			if( trim(d.congtisscpf.value) == '' ){ 
					alert('Campo obrigatório: CPF do(a) Representante da SECRETARIA DE SAÚDE'); 
					d.congtisscpf.focus();
					return false; 
			}
			if( trim(d.congtisscargofuncao.value) == '' ){ 
					alert('Campo obrigatório: Cargo/Função do(a) Representante da SECRETARIA DE SAÚDE'); 
					d.congtisscargofuncao.focus();
					return false; 
			}
			if( trim(d.congtisstelefone.value) == '' || trim(d.congtissddd.value) == '' ){ 
					alert('Campo obrigatório: (DDD) Telefone do(a) Representante da SECRETARIA DE SAÚDE'); 
					d.congtisstelefone.focus();
					return false; 
			}
			if( trim(d.congtissemail.value) == '' ){ 
					alert('Campo obrigatório: E-mail do(a) Representante da SECRETARIA DE SAÚDE'); 
					d.congtissemail.focus();
					return false; 
			}
		
			
			d.submit();
		
		}

		function abreArquivoPse()
		{
			location.href='pse.php?modulo=principal/cadastroTermoCompromisso&acao=A&arq=1';
		}
		
		<?php if($_SESSION['permissaoedicao'] == false){?>
			var obj = document.getElementsByTagName("input");
			var total = document.getElementsByTagName("input").length;
		
			for(i=0; i<total; i++){
				obj[i].disabled = true;
			}
		<?php }?>
		
		</script>
		
<?php }//fecha if($_SESSION['mensagem'] == true){?>		




<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>

var countModal = 1;

//montaShowModal();

function montaShowModal() {
	var campoTextArea = ''+
		'<p align=\'justify\'>IMPORTANTE – LEMBRAMOS QUE O PRAZO DE PACTUAÇÃO DO TERMO DE COMPROMISSO DO PSE ENCERRA-SE NO DIA 30/11/2011 ÀS 23h:59m.'+
		'<br><br><b>Atenciosamente,</b>'+
		'<br><b>Equipe PSE.</b>';
	var alertaDisplay = '<div class="titulo_box" ><b>Prezado usuário do SIMEC/PSE,</b> <br/ >'+campoTextArea+'</div><div class="links_box" ><center><input type="button" onclick=\'closeMessage(); return false \' value="Fechar" /></center>';
	displayStaticMessage(alertaDisplay,false);
	return false;
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(520,140);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

</script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
	