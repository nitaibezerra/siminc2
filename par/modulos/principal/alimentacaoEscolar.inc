<?php

/* TRECHO DO POPUP DO HISTORICO DE MODIFICACAO */
if( ! $_SESSION['par']['inuid'])
{
	echo "<script>
				alert('Faltam dados na sessão!');
				window.location.href = '	par.php?modulo=inicio&acao=C';
			  </script>";
	die();
}

if($_REQUEST['hdpid']) {
	
$arrDados = $db->pegaLinha("SELECT hdpdados, usu.usunome, to_char(hpddatainsercao,'dd/mm/YYYY HH24:MI') as hpddatainsercao FROM par.historicodadospar hdp 
							INNER JOIN seguranca.usuario usu ON usu.usucpf = hdp.usucpf 
							WHERE hdpid='".$_REQUEST['hdpid']."'");


$dados = unserialize($arrDados['hdpdados']);



$sql = "SELECT sgmid as codigo, sgmdsc as descricao FROM par.dadosunidadesegmento ORDER BY sgmdsc ASC";
$segmentos = $db->carregar($sql);
if($segmentos[0]) {
	foreach($segmentos as $seg) {
		$_segmentos[$seg['codigo']] = $seg['descricao'];
	}
}



if($dados['duncpf']) {
	foreach($dados['duncpf'] as $key => $cpf) {
		$htmltable .= "<tr>
							<td align=center>".$cpf."</td>
							<td align=center>".$dados['dunnome'][$key].((!$dados['ardun'][$key])?" (<b>COORDENADOR</b>)":"")."</td>
							<td align=center>".$dados['dunfuncao'][$key]."</td>
							<td align=center>".$_segmentos[$dados['sgmid'][$key]]."</td>
							<td align=center>".$dados['dunemail'][$key]."</td>
					   </tr>";
	}
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr><td class="SubTituloCentro" colspan="5">DADOS MODIFICADOS POR <?=$arrDados['usunome'] ?>, <?=$arrDados['hpddatainsercao'] ?></td></tr>
<tr>
				<td class="SubTituloCentro">CPF</td>	
				<td class="SubTituloCentro">Nome</td>	
				<td class="SubTituloCentro">Função/Cargo</td>	
				<td class="SubTituloCentro">Segmento</td>	
				<td class="SubTituloCentro">E-mail</td>	
</tr>
<?=$htmltable ?>
<tr><td class="SubTituloCentro" colspan="5"><input type="button" name="fechar" value="Fechar" onclick="window.close();"></td></tr>
</table>	
<?
exit;
}

/* FIM - TRECHO DO POPUP DO HISTORICO DE MODIFICACAO */


require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "includes/classes/entidades.class.inc";
require_once APPRAIZ . "www/includes/webservice/cpf.php";

	function excluirDadosUnidade($dunid = null)
	{
		
		$ajax = false;
		global $db;
		
		if( ! $dunid )
		{
			$ajax = true;
			ob_clean();
			extract($_POST);
			//ver($_POST,d);
		}
		if($dunid){
			
			
			$obDadosUnidade = new DadosUnidade($dunid);
			// Verifica se este usuário está associado a outra unidade ou cargo
			$emailExclusao = $obDadosUnidade->dunemail;
			
			$cpfExclusao = $db->pegaUm("SELECT duncpf FROM par.dadosunidade WHERE dunid = {$dunid}");
			
			$obDadosUnidade->excluir($dunid);
			
			if($obDadosUnidade->commit()){
				
				
				$vnid = $db->pegaUm("select vnid from par.vinculacaonutricionista where vncpf = '{$cpfExclusao}' and inuid = {$_SESSION['par']['inuid']} and vnstatus = 'A' ");
				
				
				if($vnid)
				{
					$sqlDesvincula = "
						UPDATE 
							par.vinculacaonutricionista
						SET
							vndatadesvinculacao = 'now()',
							usucpfalteracao = '{$_SESSION['usucpf']}',
							vnstatus = 'I'
							
						WHERE
							vnid = {$vnid}
					";
					
					$db->executar($sqlDesvincula);
				}
				
				$sqlDU = "
				SELECT
					dunid
				FROM
					par.dadosunidade
				WHERE
				
					duncpf = '{$cpfExclusao}'
				AND
					dunid <> {$dunid} ";
				
				$resultDU = $db->carregar ($sqlDU);
				
				// Caso não exista  exclui DU e os demais vínculos
				if( ! $resultDU)
				{
					
					$sqlDN =
					"
						UPDATE par.dadosnutricionista SET
							dnstatus = 'I'
						WHERE
							dncpf = '{$cpfExclusao}'
						RETURNING entid
						";
							
					$entId =  $db->pegaUm( $sqlDN );
					
					if($entId)
					{
						$sqlE =
						"
						UPDATE entidade.entidade SET
							entstatus = 'I'
						WHERE
							entid = {$entId}
						";
					
						$db->executar($sqlE);
						
						$updatePerfilUsu = "
						DELETE FROM 
							seguranca.perfilusuario
						WHERE 
							usucpf = '{$cpfExclusao}'
						AND
							pflcod =
						".PAR_PERFIL_NUTRICIONISTA;
						
						$db->executar( $updatePerfilUsu );
						
					}
						
				}
				
				if($db->commit())
				{
					
				
						if($_SESSION['par']['muncod']) {
								
							$munDesc = $db->pegaUm(" select mundescricao from territorios.municipio  where muncod = '{$_SESSION['par']['muncod']}'");
								
							$municipio = $munDesc .'-'. $_SESSION['par']['estuf'];
							$tipoEsfera = 'Município';
							$unidade = $municipio;
							$strAssunto =  "Desvinculação do Programa Alimentação Escolar – " . $municipio ;
						} else {
					
							$estado = $db->pegaUm(" select estdescricao from territorios.estado  where estuf = '{$_SESSION['par']['estuf']}'");
							$tipoEsfera = 'Estado';
							$unidade = $estado;
							$strAssunto =  "Desvinculação do Programa Alimentação Escolar – " . $estado ;
						}
$strMensagem =
"
<pre align=\"center\" style=\"text-align: justify;\"  >
	Informamos que você foi desvinculado do Quadro Técnico de nutricionistas do Programa Nacional de Alimentação Escolar no {$tipoEsfera} de {$unidade}.

Atenciosamente,
Equipe ". SIGLA_SISTEMA. "/PAR.
</pre>";
					
					$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
					$strMensagem = html_entity_decode($strMensagem);
						
					
					if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
					{
							$strEmailTo = $emailExclusao;
							// Conforme solicitação da manuelita, enviar para o email real no simec-d
					$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
						
					}
					else if($_SERVER['HTTP_HOST'] == "simec-local" )
					{
					/// não envia
						
					}
					else
					{
						$strEmailTo = $dunemail;
						$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
					}
					
					
					
				}
				
				if($ajax)
				{
					echo 'sucesso';
				}
				else
				{
					return false;
				}		
			} else {
				$obDadosUnidade->rollback();
			}
			unset($obDadosUnidade);		
		}
		else
		{
			echo 'O Nutricionista ainda não havia sido cadastrado.';
			die();
		}
		if($ajax)
		{
			die;
		}
	
	}
	function verificaUnidades()
	{
		
		global $db;
		ob_clean();
		
		$duncpf 	= str_replace("/","",str_replace("-","",str_replace(".","",$_POST['cpf'])));
		$inuid = $_SESSION['par']['inuid'];
		
		$sqlExiste = "select dunid from par.dadosunidade  where duncpf = '{$duncpf}' and inuid =  {$inuid}";
		
		$existeNessaUnidade = $db->pegaUm($sqlExiste);
		
		if($existeNessaUnidade)
		{
			echo 'existe_inuid';
			die();
			
		}		
		$sql = "Select  
				CASE WHEN inu.estuf IS NULL THEN
					m.mundescricao || '-' || inu.mun_estuf
				ELSE
					estdescricao
			
				END AS descunidade 
				
				from par.dadosunidade  du
				LEFT JOIN par.instrumentounidade inu ON du.inuid = inu.inuid
				left JOIN territorios.municipio m ON m.muncod = inu.muncod
				left JOIN territorios.estado e ON e.estuf = inu.estuf
				where dutid in ( 11, 12 ) 
				and inu.inuid <> {$_SESSION['par']['inuid']} --@todo
				and duncpf = '$duncpf'";
	
		
		$unidades = $db->carregar($sql);
		
		if( $unidades )
		{
			foreach($unidades as $v)
			{
				$arrUnidades[] = $v['descunidade'];
			}

			$unidades = implode(', ', $arrUnidades);
			
			echo("O CPF '{$_POST['cpf']}' já está vinculado as entidades a seguir: $unidades. Você deseja continuar o cadastro deste CPF? ");
			die();
		}
		else
		{
			echo 'novo';
			die();			
		}
	}

	function transfereTecnico()
	{
		ob_clean();
		
		global $db;
		$id 	= $_POST['dunid'];
		$email 	= $_POST['email'];
		
		$dados = $db->pegaLinha("SELECT duncpf, inuid from par.dadosunidade where dunid = {$id}");
		
		$cpf = $dados['duncpf'];
		$inuid = $dados['inuid'];
		
		
		$vnid =  $db->pegaUm("select vnid from par.vinculacaonutricionista  where vncpf = '{$cpf}' and inuid = {$inuid} and vnstatus = 'A' ");
		
		if($vnid)
		{
			$sql1 = "
					UPDATE
						 par.vinculacaonutricionista
					SET
					 dutid=  11 
					WHERE
						vnid = {$vnid}
			";
		
			$db->executar($sql1);
		}
		$sql = "UPDATE
					par.dadosunidade 
				SET
				 dutid=  11 
				WHERE
					 
				dunid = {$id}";
	
		$db->executar($sql);
		
		
		if( $db->commit())
		{
			if($email)
			{
				
				if($_SESSION['par']['muncod']) {
						
					$munDesc = $db->pegaUm(" select mundescricao from territorios.municipio  where muncod = '{$_SESSION['par']['muncod']}'");
						
					$municipio = $munDesc .'-'. $_SESSION['par']['estuf'];
					$tipoEsfera = 'Município';
					$unidade = $municipio;
					$strAssunto =  "Cadastro de Nutricionista para Programa Alimentação Escolar – " . $municipio ;
				} else {
			
					$estado = $db->pegaUm(" select estdescricao from territorios.estado  where estuf = '{$_SESSION['par']['estuf']}'");
					$tipoEsfera = 'Estado';
					$unidade = $estado;
					$strAssunto =  "Cadastro de Nutricionista para Programa Alimentação Escolar – " . $estado ;
				}
					
						//>
						$strMensagem =
						"
<pre align=\"center\" style=\"text-align: justify;\"  >
	Você foi cadastrado como Responsável Técnico no âmbito do Programa Nacional de Alimentação Escolar no {$tipoEsfera} de {$unidade}.
Por favor, acesse o link a seguir para acessar o SIMEC/Módulo PAR, insira seu CPF, senha e clique no botão <span style=\"width: 100px; color: white; background-color:#47A447; border: 0px;\"  >  Entrar  </span>. Confirme que
você atua nesta função clicando no botão \"Validar\".  Caso não atue nesta função, escolha a opção \"Não validar\".

http://simec.mec.gov.br 

Atenciosamente,
Equipe ". SIGLA_SISTEMA. "/PAR.
</pre>";
					
					
				$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
				$strMensagem = html_entity_decode($strMensagem);
				$strEmailTo = $email;
				
				if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
				{
						
					// Conforme solicitação da manuelita, enviar para o email real no simec-d
					$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
					
				}
				else if($_SERVER['HTTP_HOST'] == "simec-local" )
				{
					//	/ não envia
				}
				else
				{
					$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
				}
			}
			
			echo 'sucesso';
			die();
		}
		else
		{
			
			echo 'erro';
			die();			
		}
		
	}
	
	function buscaCPFlocal()
	{
		
		global $db;
		ob_clean();
		
		$duncpf 	= str_replace("/","",str_replace("-","",str_replace(".","",$_POST['cpf'])));
		
	
		
		$sql = "select usunome from seguranca.usuario  where usucpf = '$duncpf'";
		
		$nome = $db->pegaUm($sql);
		
		if( $nome  != '')
		{
			echo $nome;
			die();
		}
		else
		{
			echo 'vazio';
			die();
		}
		
	}
	
if($_POST['requisicao'] == 'excluirDadosUnidade')
{
	excluirDadosUnidade();
}
if($_POST['requisicao'] == 'transfereTecnico')
{
	transfereTecnico();
}
if($_POST['requisicao'] == 'verificaUnidades')
{
	verificaUnidades();
}
if($_POST['requisicao'] == 'buscaCPFlocal')
{
	buscaCPFlocal();
}

$inuid = $_SESSION['par']['inuid'];

$oEntidade = new Entidades();
$oDadosUnidade = new DadosUnidade();
$oDadosUnidade->recuperarHistoricoEntidadeDadosUnidade($_SESSION['par']['muncod'], $_SESSION['par']['estuf'], $_SESSION['par']['itrid'], true);
//$boBotaoVoltar = $oDadosUnidade->verificaAbasDesatualizada($inuid, $_SESSION['par']['muncod'], $_SESSION['par']['estuf'], $_SESSION['par']['itrid']);

if($_POST['formulario'] && $_POST['duncpf']){
	
	// Caso o _POST['duncpf'][0] for diferente do que temos no BD, deve-se excluir os demais vínculos
	$arDadosCoordenador = $oDadosUnidade->recuperarDadosPorInuid($inuid, ALIMENTACAO_ESCOLAR_TECNICO_NUTRICIONISTA);
	$arDadosCoordenador = $arDadosCoordenador ? $arDadosCoordenador : Array();
	$cpfAntigo = $arDadosCoordenador[0]['duncpf'];
	$cpfNovo = str_replace("/","",str_replace("-","",str_replace(".","",$_POST['duncpf'][0]))) ;
	
	if( ($cpfAntigo != $cpfNovo) && (count($arDadosCoordenador) > 0 ) )
	{
 		$dunidExc = $arDadosCoordenador[0]['dunid'];
 		
 		excluirDadosUnidade($dunidExc);
 		
	}
	
	$nutInu = $db->carregar("Select distinct duncpf, dunemail from par.dadosunidade where dutid in ( 11, 12 )  and inuid = {$_SESSION['par']['inuid']}
	");
	
	$nutInu = ($nutInu) ? $nutInu : array();
	if( count($nutInu) > 0  )
	{
		foreach($nutInu as $k => $v)
		{
			$arrNutsInu[] = $v['duncpf'];	
			$arrNutsEmail[$v['duncpf']] =  $v['dunemail'];		
		}
	}
	
	$arrNutsInu		= ($arrNutsInu)		? $arrNutsInu 	: Array();
	$arrNutsEmail	= ($arrNutsEmail) 	? $arrNutsEmail : Array();
	$oDadosUnidade->deletarDadosPorDutid($_SESSION['par']['inuid'], ALIMENTACAO_ESCOLAR_TECNICO_NUTRICIONISTA);
	$oDadosUnidade->deletarDadosPorDutid($_SESSION['par']['inuid'], ALIMENTACAO_ESCOLAR_NUTRICIONISTA);

	
	for ($i = 0; $i < count($_POST['duncpf']); $i++) {
		
		if($_POST['duncpf'][$i]){
			
			$inserePerfil = false;
			$enviaEmail = true;
					
			if(!$_POST['dunemail'][$i]){
				$msg .= 'O campo Email é obrigatório.\n';
				$boSalvou = false;		
			}
			if(!$_POST['dunnome'][$i]){
				$msg .= 'O campo Nome é obrigatório.\n';
				$boSalvou = false;		
			}
			
			if($msg){
				break;
			}
			
			
			$duncpf 	= str_replace("/","",str_replace("-","",str_replace(".","",$_POST['duncpf'][$i])));
			$dunemail 	= $_POST['dunemail'][$i];
			$dunnome 	= $_POST['dunnome'][$i];
			
			
			
			
			$oDadosUnidade->dunid 		= null;
			$oDadosUnidade->inuid 		= $_SESSION['par']['inuid'];
			$oDadosUnidade->dundata 	= date('Y-m-d H:i:s');
			$oDadosUnidade->usucpf 		= $_SESSION['usucpf'];
			if($i != 0){
				$oDadosUnidade->dutid 		= ALIMENTACAO_ESCOLAR_NUTRICIONISTA;
				$dutid				 		= ALIMENTACAO_ESCOLAR_NUTRICIONISTA;
				$funcaoUsu = 'Nutricionista';
				$funcaoUsuEmail = 'Nutricionista';
				$carid = 27;
			}else{
				$oDadosUnidade->dutid 		= ALIMENTACAO_ESCOLAR_TECNICO_NUTRICIONISTA;
				$dutid				 		= ALIMENTACAO_ESCOLAR_TECNICO_NUTRICIONISTA;
				$funcaoUsu = 'Responsável Técnico/Nutricionista';
				$funcaoUsuEmail = 'Responsável Técnico';
				$carid = 26;
			}
			$oDadosUnidade->duncpf 		= $duncpf;
			$oDadosUnidade->dunemail 	= $dunemail;
			$oDadosUnidade->dunnome 	= $dunnome;
			
			$oDadosUnidade->salvar();
			//Insere na entidade.entidade
			
			$resultEnt = $db->carregar ("select entid from entidade.entidade where entnumcpfcnpj = '{$duncpf}' ");
			if($resultEnt)
			{
				$sql = "
				UPDATE entidade.entidade SET
					entnumcpfcnpj = '{$duncpf}',
					entnome = '{$dunnome}',
					entemail = '{$dunemail}',
					entstatus = 'A'
				WHERE 
					entnumcpfcnpj = '{$duncpf}'
					
				RETURNING entid;
				";
					
				$entId =  $db->pegaUm( $sql );
				
			}
			else
			{
				$sql = "
				INSERT INTO entidade.entidade
				(
					entnumcpfcnpj,
					entnome,
					entemail,
					entstatus
				)
				VALUES
				(
					'{$duncpf}',
					'{$dunnome}',
					'{$dunemail}',
					'A'
				)
				RETURNING entid;
				";
					
				$entId =  $db->pegaUm( $sql );
			}
			
			
			
			$resultNutri = $db->carregar ("select entid from par.dadosnutricionista where dncpf = '{$duncpf}' ");
			if($resultNutri)
			{
				$sql = "
				UPDATE par.dadosnutricionista SET
					dncpf = '{$duncpf}',
					entid = '{$entId}',
					dnstatus = 'A'
				
				WHERE 
					dncpf = '{$duncpf}'
				";
			}
			else
			{
				$sql = "
					INSERT INTO par.dadosnutricionista
					(	
						dncpf,
						entid,
						dnstatus
					)
					VALUES
					(
						'{$duncpf}',
						{$entId},
						'A'
					)
					
				";
			}
			$db->executar( $sql );
			
			
			$resultNutri = $db->carregar ("select vnid from par.vinculacaonutricionista where vncpf = '{$duncpf}' and inuid = {$_SESSION['par']['inuid']} and vnstatus = 'A' ");
			if( ! $resultNutri )
			{
				
				$sql = "
				INSERT INTO par.vinculacaonutricionista
				(
						vncpf,
						inuid,
						vnstatus,
						snid,
						dutid
				)
						VALUES
						(
						'{$duncpf}',
						{$_SESSION['par']['inuid']},
						'A',
						3,
						{$dutid}
				)
				";
			}
			$db->executar( $sql );
			
			
			
			// Verifica se o cpf já existe como usuário
			$resultUsuario = $db->carregar ("select usucpf from  seguranca.usuario where usucpf = '{$duncpf}' ");
			// Caso não seja usuário
			if( ! $resultUsuario)
			{
				$senhageral = 'nut'  . substr( $duncpf, -3);
				$senha = md5_encrypt_senha( $senhageral , '' );
				
				$insertUsu = 
					"INSERT INTO seguranca.usuario (
						usucpf, usunome, usuemail,  
						usufuncao, carid, usuchaveativacao, ususenha, suscod
					) values (
						'{$duncpf}', '{$dunnome}', '{$dunemail}', '{$funcaoUsu}', {$carid},
						'F', '{$senha}', 'A'
					)";
				
				$result = $db->executar( $insertUsu );
				
				
				$insertUsuSistema = "INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod ) values ( '{$duncpf}', 23 ,
				".PAR_PERFIL_NUTRICIONISTA.")" ;

				$result = $db->executar( $insertUsuSistema );
				
				if($result)
				{
						$enviaEmail = true;
						$novoUsu = true;
				}
				
				$resultPFL = $db->carregar("select usucpf from seguranca.perfilusuario WHERE usucpf = '{$duncpf}' AND pflcod = ".PAR_PERFIL_NUTRICIONISTA);
				if( ! $resultPFL)
				{
						
					$inserePerfil = true;
				}
				
			}
			else // caso já seja
			{
				
				// verifica se possui o perfil do par
				$usuPar = $db->pegaUm("select usucpf from seguranca.usuario_sistema  where usucpf =  '{$duncpf}'  and sisid = 23 
					");
				
				// Caso não tenha, adiciona
				if(! $usuPar)
				{
					
					$insertUsuPAR = "INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod ) values ( '{$duncpf}', 23 ,
					".PAR_PERFIL_NUTRICIONISTA.")" ;
					
					$db->executar($insertUsuPAR);
				}
				// adiciona perfil
				$resultPFL = $db->carregar("select usucpf from seguranca.perfilusuario WHERE usucpf = '{$duncpf}' AND pflcod = ".PAR_PERFIL_NUTRICIONISTA);
				if( ! $resultPFL)
				{
					
					$inserePerfil = true;
					$enviaEmail = true;
					$novoUsu = false;
				}
				
			}
			
			if($inserePerfil)
			{
				$insertPerfilUsu = "INSERT into seguranca.perfilusuario
				(	usucpf,
				pflcod
				)
				VALUES
				(
				'{$duncpf}',
				".PAR_PERFIL_NUTRICIONISTA."
				)";
				$db->executar( $insertPerfilUsu );
			}
			
			if(in_array($duncpf, $arrNutsInu))
			{
				
				if($arrNutsEmail[$duncpf] == $dunemail)
				{
					$enviaEmail = false;
					 
				}
				
			}
				
			
			if($enviaEmail)
			{
				
				if($_SESSION['par']['muncod']) {
					
					$munDesc = $db->pegaUm(" select mundescricao from territorios.municipio  where muncod = '{$_SESSION['par']['muncod']}'");
					
					$municipio = $munDesc .'-'. $_SESSION['par']['estuf'];
					$tipoEsfera = 'Município';
					$unidade = $municipio;
					$strAssunto =  "Cadastro de Nutricionista para Programa Alimentação Escolar – " . $municipio ;
				} else {
						
					$estado = $db->pegaUm(" select estdescricao from territorios.estado  where estuf = '{$_SESSION['par']['estuf']}'");
					$tipoEsfera = 'Estado'; 
					$unidade = $estado;
					$strAssunto =  "Cadastro de Nutricionista para Programa Alimentação Escolar – " . $estado ;
				}
				
				
				
				
			if($novoUsu)
			{
				
				//>
				$strMensagem =
				"
				<pre align=\"center\" style=\"text-align: justify;\"  >
	Você foi cadastrado como {$funcaoUsuEmail} no âmbito do Programa Nacional de Alimentação Escolar no {$tipoEsfera} de {$unidade}.
Por favor, acesse o link a seguir para acessar o SIMEC/Módulo PAR, insira seu CPF, senha e clique no botão <span style=\"width: 100px; color: white; background-color:#47A447; border: 0px;\"  >  Entrar  </span>. Confirme que 
você atua nesta função clicando no botão \"Validar\".  Caso não atue nesta função, escolha a opção \"Não validar\".
				
http://simec.mec.gov.br
Usuário: {$duncpf}
Senha: {$senhageral}	

Atenciosamente,
Equipe ". SIGLA_SISTEMA. "/PAR.
</pre>";
			}
			else 
			{
				$strMensagem =
				"
				<pre align=\"center\" style=\"text-align: justify;\"  >
	Você foi cadastrado como {$funcaoUsuEmail} no âmbito do Programa Nacional de Alimentação Escolar no {$tipoEsfera} de {$unidade}.
Por favor, acesse o link a seguir para acessar o SIMEC/Módulo PAR, insira seu CPF, senha atuais de cadastro no SIMEC e clique no botão <span style=\"width: 100px; color: white; background-color:#47A447; border: 0px;\"  >  Entrar  </span>. Confirme que 
você atua nesta função clicando no botão \"Validar\".  Caso não atue nesta função, escolha a opção \"Não validar\".

Caso não lembre a sua senha acesse o link http://simec.mec.gov.br/recupera_senha.php
Se mesmo assim não conseguir acessar o sistema, entre em contato com:

Equipe Técnica do PAR – Plano de Ações Articuladas

Secretaria de Educação Básica
Ministério da Educação
Telefones: (61) 2022-8334/8337 / 8338/8332/9448.
Endereço eletrônico: planodemetas@mec.gov.br
	
http://simec.mec.gov.br

Atenciosamente,
Equipe ". SIGLA_SISTEMA. "/PAR.
</pre>";
			
			}


				$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
				$strMensagem = html_entity_decode($strMensagem);
					
				
				if($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
				{
					$strEmailTo = $dunemail;
					// Conforme solicitação da manuelita, enviar para o email real no simec-d
					$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
					
				}
				else if($_SERVER['HTTP_HOST'] == "simec-local" )
				{
					/// não envia	
				}
				else
				{
					$strEmailTo = $dunemail;
					$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
				}
			}
			
			//@todo jogar fora o lixo da tela e inserir o usuário corretamente
			$boSalvou = true;
			
		}
	}
	
	$sql = "INSERT INTO par.historicodadospar(
            	muncod, estuf, hdplink, hdpdados, usucpf, hpddatainsercao)
    			VALUES (".(($_SESSION['par']['muncod'])?"'".$_SESSION['par']['muncod']."'":"NULL").", 
    					".(($_SESSION['par']['estuf'])?"'".$_SESSION['par']['estuf']."'":"NULL").", 
    					'".$_SERVER['REQUEST_URI']."', '".serialize($_REQUEST)."', '".$_SESSION['usucpf']."',
    					NOW());";
		
	$db->executar($sql);
	$db->commit();

	if($boSalvou){
		$oDadosUnidade->commit();
		echo "<script>
				alert('Operação realizada com sucesso');
				window.location.href = 'par.php?modulo=principal/alimentacaoEscolar&acao=A';
			  </script>";
		exit;
	} else {
		$oDadosUnidade->rollback();
		echo "<script>
			alert('$msg');
				window.location.href = 'par.php?modulo=principal/alimentacaoEscolar&acao=A';
			  </script>";
		exit;
	}
}

print "<br />";

print carregaAbaDadosUnidade("/par/par.php?modulo=principal/alimentacaoEscolar&acao=A");

monta_titulo( "Dados da unidade", "Alimentação Escolar" );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

//Pega os perfis do usuário
$arrPerfil = pegaPerfilGeral();

function getSituacaoValidacao($cpf, $inuid)
{
	global $db;
	$result = $db->pegaUm("
	SELECT
	sn.sndescricao 
	FROM
		 par.vinculacaonutricionista  vn 
	INNER JOIN par.situacaonutricionista sn ON sn.snid = vn.snid
	WHERE
		vncpf = '{$cpf}'AND inuid = {$inuid}
	ORDER BY vnid desc
"
		);

	return $result; 
}


#Estrutura condicional mudada em 04/06/2012, para que seja habilitando o questionário somente para os perfis que tem essa permissão, permissão para edição.
#Habilitando ou desabilitando os botões "Salvar anterior, Salvar e Salvar próximo".  
#O valor "false" habilita o quetionário.
#O valor "true" desabilita o questionário.

if(	in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil) || in_array( PAR_PERFIL_EQUIPE_TECNICA, $arrPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) || in_array(PAR_PERFIL_PREFEITO, $arrPerfil)	){
	$boFormDesabilitado = false;
}else{
	$boFormDesabilitado = true;
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function(){
	var anterior = "<input type=\"button\" name=\"anterior\" value=\"<<\" onclick=\"window.location='par.php?modulo=principal/equipeLocal&acao=A'\" >";
	var proximo = "<input type=\"button\" name=\"proximo\" value=\">>\" disabled=\"disabled\" >";
	var acoes = jQuery('#tr_acoes').find('td').html();
	jQuery('#tr_acoes').find('td').html(anterior+acoes+proximo);

	jQuery('#formulario').submit(function(){

		var boSubmeter = true;
		var boDuplicado = false;
		var countCpf = 1;
		if(jQuery("input.[name=duncpf[]]").length > 0){
			jQuery("input.[name=duncpf[]]").each(function(){
				var cpfTemp = '';
				//var id = '';
				if(jQuery(this).val()){
					cpfTemp = jQuery(this);
				
					jQuery("input.[name=duncpf[]]").not(cpfTemp).each(function(){
						if(jQuery(this).val() == cpfTemp.val()){
							alert('Existe CPF duplicado.');
							cpfTemp.val('');
							cpfTemp.parent().parent().find("input[name=dunnome[]]").val('');
							cpfTemp.parent().parent().find("label").html('');
							cpfTemp.focus();
							boDuplicado = true;
							return false;
						}
					});
				}else{
					if( countCpf == 1)
					{
						alert('O campo CPF é obrigatório.');
						jQuery(this).focus();
						return false;
					}
				}
				if(boDuplicado){
					return false;
				}
				countCpf++;
			});
		}
		if(boDuplicado){
			return false;
		}
		var countNome = 1;
		jQuery("input.[name=dunnome[]]").each(function(){
			if( countNome == 1)
			{
				if(jQuery(this).val() == ''){
					alert('O campo Nome é obrigatório.');
					jQuery(this).focus();
					boSubmeter = false;
					return false;
				}
			}
			countNome++;
		});

		var countEmail = 1;
		jQuery("input.[name=dunemail[]]").each(function(){

			if(jQuery(this).val() != '')
			{
				if(!validaEmail(jQuery(this).val())){
					alert('E-mail inválido.');
					jQuery(this).focus();
					boSubmeter = false;
					return false;
				}
			}
			if( countEmail == 1)
			{
				if(jQuery(this).val() == ''){
					alert('O campo E-mail é obrigatório.');
					jQuery(this).focus();
					boSubmeter = false;
					return false;
				}
			}
			countEmail++;
		});

		return boSubmeter;

	});


	jQuery('a.exibicaoSegmento').live('click',function(){
		var td 			= jQuery(this).parent('td');
		var sgmid       = td.find('select');
		var dunsegmento = td.find('input');
		var link        = td.find('a');
		
		dunsegmento.hide();
		link.hide();
		sgmid.show();
		sgmid.val('');
	});

	var is_chrome = navigator.userAgent.toLowerCase().indexOf('chrome') > -1;
	
	// IE8 DOES NOT SUPPORT THE CHANGE EVENT WHILE USING LIVE
		if(jQuery.browser.msie){
			var evt = "click";
		}
		if(jQuery.browser.mozilla){
			var evt = "change";
		}
		if(is_chrome){
			var evt = "blur";
		}
	jQuery('select.exibicaoSegmento').live(evt,function(){
		
		if(evt=='click'){
			jQuery(this).change();
		}
				
		var td 			= jQuery(this).parent('td');
		var sgmid       = td.find('select');
		var dunsegmento = td.find('input');
		var link        = td.find('a');
	
		var objAtual = td.find('select option:selected');
		var id_option_selecionado = td.find('select option:selected').attr('id');
		if( sgmid.val() == 10 ){
			dunsegmento.show();
			dunsegmento.attr('class','');
			link.show();
			sgmid.hide();
			link.attr('class','exibicaoSegmento');
		} else if (sgmid.val() == 1){
			jQuery("select[name=sgmid[]] option:selected").not(objAtual).each(function(){
				if(jQuery(this).val() == 1){
					alert('Não é possível cadastrar dois Dirigentes.');
					sgmid.val('');
					return false;
				}
			});
		}
	});

	if(jQuery("select.[name=sgmid[]]").length > 0){
		jQuery("select.[name=sgmid[]] option:selected").each(function(){
			if(jQuery(this).val() == 10){
				jQuery('.exibicaoSegmento').change();
			}
		});
	}
	
	if(jQuery("input.[name=duncpf[]]").length > 0){
		jQuery("input.[name=duncpf[]]").each(function(){
			if(jQuery(this).val() != ''){
				jQuery(this).val(mascaraglobal('###.###.###-##',jQuery(this).val()));
			}
		});
	}
	
	jQuery('.add').click(function(){
		var $campos = jQuery('#tbodyTabela'),
				$tr = $campos.find('tr:first').clone();
				//var id_cpf = $tr.find("input.[name=duncpf[]]").attr('id');
				//var tamanho = id_cpf.length;
				//var novo_idcpf = parseInt(id_cpf.substring(4,tamanho))+2;
				//$tr.find("input.[name=duncpf[]]").attr('id','cpf_'+novo_idcpf);

				// sempre colocar o select e não o input de outros
				$tr.find('td:last').prev().find('select[name=sgmid[]]').css('display', 'inline');
				$tr.find('td:last').prev().find('input[name=dunsegmento[]]').css('display', 'none');
				$tr.find('td:last').prev().find('a.exibicaoSegmento').css('display', 'none');
				
				$tr.find("input").val("");
				$tr.find("select").val("");
				$tr.find("label").html("");
			$campos.append($tr);
		return false;
	});
	
	jQuery('.removeItens').live('click',function(){

		window.confirm( "Tem certeza que deseja excluir o nutricionista do quadro técnico?" )
		{
			var filhos = jQuery('#tbodyTabela').children().length;

			var id = jQuery(this).attr('id');
			var tr = jQuery(this).closest('tr');
			var param = new Array();
				param.push({name : 'requisicao', value : 'excluirDadosUnidade'}, 
						   {name : 'dunid', value : id}
			);
			
			if (filhos > 1) {
				
					jQuery.ajax({
						type	 : "POST",
						url		 : "par.php?modulo=principal/alimentacaoEscolar&acao=A",
						data	 : param,
						async    : false,
						success	 : function(data){
										if(trim(data) == 'sucesso'){
											tr.remove();
										}
										else
										{
											alert(data);
											tr.remove();
										}
								   }
						 });
			}
			else
			{
				jQuery.ajax({
					type	 : "POST",
					url		 : "par.php?modulo=principal/alimentacaoEscolar&acao=A",
					data	 : param,
					async    : false,
					success	 : function(data){
									if(trim(data) == 'sucesso'){
										
										window.location.href = 'par.php?modulo=principal/alimentacaoEscolar&acao=A';
									}
									else
									{
										alert(data);
										window.location.href = 'par.php?modulo=principal/alimentacaoEscolar&acao=A';
									}
							   }
					 });
			
			}
		}
		return false;
	});
	
	jQuery('input.classcpf').live(evt,function(){
		if(evt=='click'){
			jQuery(this).change();
		}

		if(jQuery(this).val() != '')
		{
			
			if( !validar_cpf( jQuery(this).val()  ) ){
				alert( "CPF inválido!\nFavor informar um cpf válido!" );
				jQuery(this).val('');
				jQuery(this).parent().parent().find("input[name=dunnome[]]").val('');
				jQuery(this).parent().parent().find("label").html('');
				return false;	
			}

			var param = new Array();
			param.push({name : 'requisicao', value : 'verificaUnidades'}, 
					   {name : 'cpf', value : jQuery(this).val()});

			var continua = true;   
			jQuery.ajax({
				type	 : "POST",
				url		 : "par.php?modulo=principal/alimentacaoEscolar&acao=A",
				data	 : param,
				async    : false,
				success	 : function(data){
								if((trim(data) != 'novo') && (trim(data) != 'existe_inuid'))
								{
									if ( ! ( confirm( data ) )) {

										continua = false;
									}
								}
						   }
				 });

			if( ! continua) 
			{
				jQuery(this).val('');
				jQuery(this).parent().parent().find("input[name=dunnome[]]").val('');
				jQuery(this).parent().parent().find("label").html('');
			}
			 
		}
		var comp  = new dCPF();
		var td 	  = jQuery(this).parent('td').next();
		var nome  = td.find('input');
		var label = td.find('label');
		//@todo 
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'buscaCPFlocal'}, 
					   {name : 'cpf', value : jQuery(this).val()});

		cpfbuscareceita =  jQuery(this).val();
		jQuery.ajax({
		type	 : "POST",
		url		 : "par.php?modulo=principal/alimentacaoEscolar&acao=A",
		data	 : param,
		async    : false,
		success	 : function(data){
						if(trim(data) == 'vazio'){

							comp.buscarDados( cpfbuscareceita );
							
							if (comp.dados.no_pessoa_rf != ''){
								nome.val(comp.dados.no_pessoa_rf);
								label.html(comp.dados.no_pessoa_rf);
								nome.attr("readonly","readonly");
							}
							
						}
						else
						{
							nome.val(trim(data));
							label.html(trim(data)); //@todo 
							nome.attr("readonly","readonly");
						}
				   }
		 });
		
		
		/*
			// Para testes locais descomente esse trecho e comente o abaixo
			nome.val('asdfasdf');
			label.html('asdfasdf'); //@todo 
			nome.attr("readonly","readonly");
		*/
	
	
	});
	
	jQuery('input.classcpf').live('blur',function(){
		var cpfAtual = jQuery(this);
		var idCpfAtual = jQuery(this).attr('id');
		var valorCpfAtual = jQuery(this).val();
		var boDuplicado = false;

		jQuery("input.[name=duncpf[]]").each(function(){
			if(jQuery(this).val()){
				//jQuery("input.[name=duncpf[]]").not('#'+idCpfAtual).each(function(){
				jQuery("input.[name=duncpf[]]").not(cpfAtual).each(function(){
					if(jQuery(this).val() && (jQuery(this).val() == valorCpfAtual)){
						alert('CPF duplicado.');
						cpfAtual.val('');
						cpfAtual.parent().parent().find("input[name=dunnome[]]").val('');
						cpfAtual.parent().parent().find("label").html('');
						cpfAtual.focus();
						boDuplicado = true;
						return false;
					}
				});
			}
			if(boDuplicado){
				return false;
			}
		});

	});
	
	<?php if($boFormDesabilitado): ?>
		jQuery('#entnumcpfcnpj').attr("disabled", true);
		jQuery('[name=cadastra],[class=removeItens],[class=add],[class=exibicaoSegmento]').remove();
		jQuery('input').attr("readonly", true);
		jQuery('select').attr("disabled", true);
		jQuery('textarea').attr("readonly", true);
		jQuery('#formulario').attr("onsubmit","return false");
		jQuery('#formulario').submit(function(){
			alert('Ação Indisponível!');
			return false;
		});
	<?php endif; ?>
	
	
});


function verModificacao(hdpid){
	window.open('par.php?modulo=principal/alimentacaoEscolar&acao=A&hdpid='+hdpid, 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
}


function excluiTecnico(id){

	if(window.confirm( "Tem certeza que deseja excluir o Responsável Técnico do quadro técnico?" ))
	{
		var param = new Array();
			param.push({name : 'requisicao', value : 'excluirDadosUnidade'}, 
					   {name : 'dunid', value : id}
		);
			
		jQuery.ajax({
			type	 : "POST",
			url		 : "par.php?modulo=principal/alimentacaoEscolar&acao=A",
			data	 : param,
			async    : false,
			success	 : function(data){
							if(trim(data) == 'sucesso'){
								
								window.location.href = 'par.php?modulo=principal/alimentacaoEscolar&acao=A';
							}
							else
							{
								alert(data);
								window.location.href = 'par.php?modulo=principal/alimentacaoEscolar&acao=A';
							}
					   }
			 });
	}
	
}

function transfereTecnico(id){

	
	email = jQuery('#tec_email_'+id).val();
	nome = jQuery('#tec_nome_'+id).val();
  	if( window.confirm( "Tem certeza que deseja alterar o nutricionista "+nome+" do Quadro Técnico para Responsável Técnico/Nutricionista?" ) )
  	{
		var param = new Array();
		param.push({name : 'requisicao', value : 'transfereTecnico'}, 
				   {name : 'dunid', value : id},
				   {name : 'email', value : email}
		);
		jQuery.ajax({
			type	 : "POST",
			url		 : "par.php?modulo=principal/alimentacaoEscolar&acao=A",
			data	 : param,
			async    : false,
			success	 : function(data){
							if(trim(data) == 'sucesso'){
								alert('Atualizado com sucesso');
								window.location.href = 'par.php?modulo=principal/alimentacaoEscolar&acao=A';
							}
					   }
			 });
  	}
}

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<?php if( $_SESSION['par']['atualizacao'] ){ ?>
	<tr>
		<td colspan="2">
			<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="60%" class="tabela">
				<tbody>
					<tr bgcolor="#cccccc">
						<td>
							<div style="float: left; width:100%; text-align: center;">
								<b>ATUALIZAÇÃO DO PAR</b><BR>
								<input value="Retornar para a Atualização do PAR" type="button" id="atualizacaopar" onclick="javascript:window.location='par.php?modulo=principal/dadosUnidadeAtualizacaoPAR&acao=A'">
							</div>
						</td>
					</tr>
				</tbody>
			</table>
		</td>
	</tr>
	<? } ?>
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">
				<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
			</a>
		</td>
		<td style="color: blue; font-size: 12px; text-align:right">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A">Voltar à Árvore |</a>
				<label style="color:black; text-align:center">
					<b>Dados da Unidade</b>
				</label>
			<a href="par.php?modulo=principal/listaObrasParUnidade&acao=A">| Lista de Obras </a>
			<a href="par.php?modulo=principal/questoesPontuais&acao=A">| Questões Pontuais</a>
		</td>
	</tr>
</table>

<?php 
# Carrega técnico nutricionista
$arDadosCoordenador = $oDadosUnidade->recuperarDadosPorInuid($inuid, ALIMENTACAO_ESCOLAR_TECNICO_NUTRICIONISTA);
$arDadosCoordenador = ($arDadosCoordenador) ? $arDadosCoordenador : array();

# Carrega Segmentos
$arSegmentos 		= $oDadosUnidade->recuperarSegmentos();
$arSegmentos 		= ($arSegmentos) ? $arSegmentos : array();
?>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="inuid" value="<?php echo $_SESSION["inuid"]; ?>"/>

	<table class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="95%" >
		<thead>
			<tr align="center" style=" background: #ccc; color: #000;">
				<td colspan="6" style="font-weight: bold; font-size: 15px;">Responsável Técnico/Nutricionista</td>	
			</tr>
			<tr align="center">
				<td width="3%">&nbsp;</td>
				<td width="12%">CPF</td>
				<td width="15%">Nome</td>
				<td width="20%">E-mail</td>
				<td width="20%">Situação</td>
			</tr>	
		</thead>
		<tbody>
			<?php 
				if($arDadosCoordenador[0])
				{
					extract($arDadosCoordenador[0]);				
				}
			?>
			<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245);" id="linha_1" align="center">
				<td>
					
					<input name="ardun[]" type="hidden" />
					
					<?php if($_SESSION['par']['boPerfilConsulta']):?>
						<img  id="<?php echo $dunid; ?>" alt="Excluir" title="Excluir" src="/imagens/excluir_01.gif">
					<?php else:?>
						<img onclick="javascript:excluiTecnico('<?php echo $dunid; ?>')" id="<?php echo $dunid; ?>" alt="Excluir" title="Excluir" src="/imagens/excluir.gif">
					<?php endif; ?>
					
					
					
				</td>
				<td>
					<input value="<?php echo $dunid; ?>" name="dunid[]" type="hidden" />
					<?php 
						$tecnicoVazio = false;	
						if(!$dunid)
						{
							$tecnicoVazio = true;
						}
					?>
					
					<input id="tec_cpf" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" class="normal classcpf" size="19" style="width: 21ex;" value="<?php echo $duncpf; ?>" name="duncpf[]" type="text" /><img title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
				<td>
					<input id="tec_nome" value="<?php echo $dunnome; ?>" name="dunnome[]" class="dunnome" type="hidden" /><label id="tec_nome_label" ><?php echo $dunnome; ?></label>
				</td>
				<td>
					<input id="tec_email" size="30" style="width: 33ex;" value="<?php echo $dunemail; ?>" name="dunemail[]" type="text" class="normal">
					<img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
				</td>
				<td>
					<label><?php echo getSituacaoValidacao($duncpf, $_SESSION['par']['inuid'])  ?></label>
				</td>
			</tr>
			<?php $count++ ?>
		</tbody>
	</table>
	<br />
	
	<table id="tabelaEquipeLocal" class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="95%" >
		<thead>
			<tr align="center" style=" background: #ccc; color: #000;">
				<td colspan="6" style="font-weight: bold; font-size: 15px;">Quadro Técnico/Nutricionistas</td>	
			</tr>
			<tr align="center">
				<td width="3%">&nbsp;</td>	
				<td width="12%">CPF</td>	
				<td width="15%">Nome</td>
				<td width="20%">E-mail</td>	
				<td width="20%">Situação</td>
			</tr>	
		</thead>
		<tbody id="tbodyTabela">
		<?php 		# Carrega nutricionista
			$dunid = null;
			$duncpf = null;
			$dunnome = null;
			$dunemail = null;
			
			$arDadosIntegrantes = $oDadosUnidade->recuperarDadosPorInuid($inuid, ALIMENTACAO_ESCOLAR_NUTRICIONISTA);
			
			$tecVazioIcone = false;
			if($tecnicoVazio)
			{
				if(is_array($arDadosIntegrantes ))
				{	
					$tecVazioIcone = true;
				}
			}
			
			 $arDadosIntegrantes = $arDadosIntegrantes ? $arDadosIntegrantes : array(array(1));

			
			 
			 ?>
		<?php $count = 1 ?>
		<?php foreach ($arDadosIntegrantes as $dados) : ?>
			<?php extract($dados);?>	
			<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245);" id="linha_1" align="center">
				<td>
					<?php if($_SESSION['par']['boPerfilConsulta']):?>
						<img alt="Excluir" title="Excluir" src="/imagens/excluir_01.gif">
					<?php else:?>
						<img class="removeItens" id="<?php echo $dunid; ?>" alt="Excluir" title="Excluir" src="/imagens/excluir.gif">
					<?php endif; ?>
					<?php 
					
						if($tecVazioIcone)
						{
							echo "<a href=\"javascript:transfereTecnico({$dunid});\"> <img title=\"Transferir para Responsável Técnico\" src=../imagens/seta_verde.png style=cursor:pointer;></a>";
						}

					
					?>
					<input value="1" name="ardun[]" type="hidden" />
				</td>
				<td>
					<input value="<?php echo $dunid; ?>" name="dunid[]" type="hidden" />
					<input id="tec_cpf_<?= $dunid?>" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" class="normal classcpf" size="19" style="width: 21ex;" value="<?php echo $duncpf; ?>" name="duncpf[]" type="text" /><img title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
				<td>
					<input id="tec_nome_<?= $dunid?>" value="<?php echo $dunnome; ?>" name="dunnome[]" class="dunnome" type="hidden" /><label id="tec_nome_<?= $dunid?>" ><?php echo $dunnome; ?></label>
				</td>
				<td>
					<input size="30" id="tec_email_<?= $dunid?>" style="width: 33ex;" value="<?php echo $dunemail; ?>" name="dunemail[]" type="text" class="normal">
					<img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
				</td>
				<td>
					<label id="tec_situacao_<?= $dunid?>" ><?php echo getSituacaoValidacao($duncpf, $_SESSION['par']['inuid'])  ?></label>
				</td>
			</tr>
			<?php $count++ ?>
		<?php endforeach; ?>
		</tbody>
	</table>
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td style="">
				<?php if($_SESSION['par']['boPerfilConsulta']):?>
					<span style="margin-left:5px; cursor:pointer">
						<img src="/imagens/gif_inclui_d.gif" align="top" style="border: none" />
						Inserir Integrante
					</span>
				<?php else:?>
					<span class="add" style="margin-left:5px; cursor:pointer">
						<img src="/imagens/gif_inclui.gif" align="top" style="border: none" />
						Inserir Integrante
					</span>
				<?php endif;?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0" align="center">
			<td>
				<input type="button" class="botao" name="anterior" value='Anterior' onclick="window.location='par.php?modulo=principal/equipeLocal&acao=A'" >
				<?php if(!$_SESSION['par']['boPerfilConsulta']):?>
					<input type='submit' class="botao" name='cadastra' value='Salvar' />
				<?php endif; ?>
				<!-- input type='button' class="botao" name='fecha' value='Voltar' onclick="fechar();"/ -->
				<input type="button" class="botao" name="proximo" value='Próximo' disabled="disabled" >
			</td>
		</tr>
	</table>

	<?php //if(count($arHistoricoCoordenador) || count($arHistoricoIntegrantes)): ?>
	<!--<table align="center" width="95%;" style="margin-top:10px">
		<tr bgcolor="#c0c0c0">
			<td colspan="2" align="center"><b>Últimas alterações:</b></td>
		</tr>
		<?php //if(count($arHistoricoCoordenador)): ?>
		<tr>
			<td align="left" width="100">
				Coordenador:
			</td>
			<td align="left">
				<?php //echo $arHistoricoCoordenador['dunnome'] ?> em 
				<?php //echo formata_data($arHistoricoCoordenador['dundata']) ?>
			</td>
		</tr>
		<?php //endif; ?>
		<?php //if(count($arHistoricoIntegrantes)): ?>
		<tr>
			<td align="left">
				Integrantes:
			</td>
			<td align="left">
				<?php //echo $arHistoricoIntegrantes['dunnome'] ?> em 
				<?php //echo formata_data($arHistoricoIntegrantes['dundata']) ?>
			</td>
		</tr>
		<?php //endif; ?>
	</table>
	--><?php //endif; ?>
</form>
<div id="divDebug"></div>
<?

monta_titulo( "Histórico de modificações", "Comite Local" );
							 
$sql = "SELECT '<center><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"verModificacao(\''||hdp.hdpid||'\');\"></center>' as acao, usu.usunome, to_char(hdp.hpddatainsercao, 'dd/mm/YYYY HH24:MI') as hpddatainsercao 
		FROM par.historicodadospar hdp
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = hdp.usucpf 
		WHERE hdplink='/par/par.php?modulo=principal/alimentacaoEscolar&acao=A' ".(($_SESSION['par']['itrid']==2)?"AND hdp.muncod='".$_SESSION['par']['muncod']."' ":"").(($_SESSION['par']['itrid']==1)?"AND hdp.estuf='".$_SESSION['par']['estuf']."' AND hdp.muncod IS NULL ":"");

$cabecalho = array("&nbsp;","Nome","Data alteração");
$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%',$par2);


?>