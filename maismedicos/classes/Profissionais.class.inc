<?php

class Profissionais extends Modelo{
	
	public function recuperaUsuariosExistentesNovosSupTutor()
	{
		$sql = "select * from (
		
					select distinct tutcpf as cpf, tutnome as nome, tutemail as email, tuttipo as tipo, '' as sexo, muncod,
						substr(replace(replace(replace(replace(tuttelefone,'-',''),' ',''),'(',''),')',''),0,3) as ddd,
						substr(replace(replace(replace(replace(tuttelefone,'-',''),' ',''),'(',''),')',''),3,15) as fone, tuttelefone as foneo,
						tutdatanascimento as data_nascimento
					from maismedicos.tutor where tutstatus = 'A' and tutvalidade = true and tutcpf is not null
				) as foo
				where trim(cpf) not in (select distinct usucpf from seguranca.usuario)
				order by nome;";
		
		$rsNovos = $this->carregar($sql);
		
		$sql = "select usucpf as cpf, usunome as nome, usufuncao as tipo seguranca.usuario
				where usucpf in (
					select distinct tutcpf as cpf from maismedicos.tutor
					where tutstatus = 'A' and tutvalidade = true and tutcpf is not null
				) and (suscod != 'A' or usustatus != 'A');";
		
		$rsExistentes = $this->carregar($sql);
		
		// Insere novos usuarios
		if($rsNovos){
			
			$txt .= "<h1>Importa usuários (tutores e supervisores) do UNASUS</h1>
				<p>".count($rsNovos)." novos usuários</p>
				<p>".count($rsExistentes)." usuários ativados</p>";
				
			$txt .= '<h3>Usuários novos</h3>
					<table class="listagem" cellpadding="3" cellspacing="1" width="95%" align="center" style="font-size:8px;">
						<tr bgcolor="#c0c0c0">
							<td><b>CPF</b></td>
							<td><b>Nome</b></td>
							<td><b>Função</b></td>
						</tr>';
			foreach($rsNovos as $importa){
				$txt .= '<tr>
							<td>'.$importa['cpf'].'</td>
							<td>'.$importa['nome'].'</td>
							<td>'.$importa['tipo'].'</td>
						</tr>';
			}
			$txt .= '</table>';
		}
		
		// Ativa usuraios existentes
		if($rsExistentes){
				
			$txt .= '<h3>Usuários ativados</h3>
					<table class="listagem" cellpadding="3" cellspacing="1" width="95%" align="center" style="font-size:8px;">
						<tr bgcolor="#c0c0c0">
							<td><b>CPF</b></td>
							<td><b>Nome</b></td>
							<td><b>Função</b></td>
						</tr>';
			foreach($rsExistentes as $atualiza){
				$txt .= '<tr>
							<td>'.$atualiza['cpf'].'</td>
							<td>'.$atualiza['nome'].'</td>
							<td>'.$atualiza['tipo'].'</td>
						</tr>';
			}
			$txt .= '</table>';
		}
		
		return $txt;
	}
	
	public function importaUsuariosSupervisoresTutoresUnasus()
	{
		
		$txt = "<h2>Importa usuários (tutores e supervisores) Mais Médicos</h2>";
		
		$sql = "-- Ativa usuarios para bolsistas bloqueados, inativos ou pendentes
					update seguranca.usuario  set suscod = 'A', usustatus = 'A' where usucpf in (
						select distinct tutcpf as cpf from maismedicos.tutor where tutstatus = 'A' and tutvalidade = true and tutcpf is not null
					)
					--and (suscod != 'A' or usustatus != 'A');";
			
		$n = $this->executar($sql);
		$totalUsuariosExistentes = pg_affected_rows($n);
		
		$sql = "-- Ativa sistema mais medicos para bolsistas bloqueados, inativos ou pendentes
					update seguranca.usuario_sistema  set suscod = 'A', susstatus = 'A' where usucpf in (
						select distinct tutcpf as cpf from maismedicos.tutor where tutstatus = 'A' and tutvalidade = true and tutcpf is not null
					)
					and sisid = (select sisid from seguranca.sistema where sisdiretorio = 'maismedicos')
					--and (susstatus != 'A' or suscod != 'A');";
			
		$n = $this->executar($sql);
		$totalSistemaUsuariosExistentes = pg_affected_rows($n);
		
		$sql = "select distinct
			
					cpf,nome,email,'A' as status,ddd,fone,'XPmazb9p/0OWZVj/oCq5aeUvv+26NqEE6YAJmOz4Qn4=' as senha, --senha de 1a6
					'Carga dos bolsitas do projeto Mais Médicos, usuário inserido em ' || to_char(now(), 'DD/MM/YYYY') || '.' as obs,
					case when tipo = 'S' then 'Supervisor Mais Médicos' when tipo = 'T' then 'Tutor Mais Médicos' when tipo = 'M' then 'Médico Mais Médicos' end as funcao,
					sexo,case when tipo = 'S' then 1343 when tipo = 'T' then 1220 when tipo = 'M' then null end as pflcod,'A' as suscod,muncod,data_nascimento,null as tpocod,
					false,now()
			
				from (
			
					select distinct tutcpf as cpf, tutnome as nome, tutemail as email, tuttipo as tipo, '' as sexo, muncod,
					substr(replace(replace(replace(replace(tuttelefone,'-',''),' ',''),'(',''),')',''),0,3) as ddd,
					substr(replace(replace(replace(replace(tuttelefone,'-',''),' ',''),'(',''),')',''),3,15) as fone, tuttelefone as foneo,
					tutdatanascimento as data_nascimento from maismedicos.tutor where tutstatus = 'A' and tutvalidade = true and tutcpf is not null
			
				) as foo
				where trim(cpf) not in (select distinct trim(usucpf) from seguranca.usuario);";
		
		$rsUsuarios = $this->carregar($sql);
		
		if($rsUsuarios){
						
			foreach($rsUsuarios as $usu){
				if($usu['cpf']){
					
					$senha = strtolower(senha());
					$senha = md5_encrypt_senha($senha, '');
					
					$arSqlUsu[] = "
							('{$usu['cpf']}','{$usu['nome']}','{$usu['email']}','{$usu['status']}','{$usu['ddd']}',
							'{$usu['fone']}','{$senha}','{$usu['obs']}','{$usu['funcao']}','{$usu['sexo']}','{$usu['pflcod']}',
							'{$usu['suscod']}','{$usu['muncod']}','{$usu['data_nascimento']}','{$usu['tpocod']}',false,now())";
				}
			}
			
			if($arSqlUsu){
				$sqlUsu = 'insert into seguranca.usuario (usucpf,usunome,usuemail,usustatus,usufoneddd,usufonenum,ususenha,usuobs,usufuncao,
										ususexo,pflcod,suscod,muncod,usudatanascimento,tpocod,usuchaveativacao,usudataatualizacao) 
							values'.implode(',', $arSqlUsu).';';
			}
		}
		
		if($sqlUsu){
			$n = $this->executar($sqlUsu);
			$totalUsuariosNovos = pg_affected_rows($n);
		}else{
			$totalUsuariosNovos=0;
		}
		
		$sql = "--Insere os usuarios no sistema 
					insert into seguranca.usuario_sistema (usucpf, sisid, susstatus, suscod,pflcod)
					select distinct
		
						cpf,(select sisid from seguranca.sistema where sisdiretorio = 'maismedicos'),'A','A',case when tipo = 'S' then 1343 when tipo = 'T' then 1220 when tipo = 'M' then null end as pflcod --sistema
			
					from (
		
						select distinct tutcpf as cpf, tutnome as nome, tutemail as email, tuttipo as tipo, '' as sexo, muncod,
						substr(replace(replace(replace(replace(tuttelefone,'-',''),' ',''),'(',''),')',''),0,3) as ddd,
						substr(replace(replace(replace(replace(tuttelefone,'-',''),' ',''),'(',''),')',''),3,15) as fone, tuttelefone as foneo,
						tutdatanascimento as data_nascimento from maismedicos.tutor where tutstatus = 'A' and tutvalidade = true and tutcpf is not null
						and tutcpf not in (select usucpf from seguranca.usuario_sistema  
										where sisid = (select sisid from seguranca.sistema where sisdiretorio = 'maismedicos'))
					) as foo;";
		
		$n = $this->executar($sql);
		$totalSistemaUsuariosNovos = pg_affected_rows($n);
		
		
		
		$sql = "--Insere os perfis para os usuarios
					insert into seguranca.perfilusuario (usucpf, pflcod)
					select distinct
		
						cpf,case when tipo = 'S' then 1343 when tipo = 'T' then 1220 when tipo = 'M' then null end as pflcod --perfil
		
					from (
		
						select distinct tutcpf as cpf, tutnome as nome, tutemail as email, tuttipo as tipo, '' as sexo, muncod,
						substr(replace(replace(replace(replace(tuttelefone,'-',''),' ',''),'(',''),')',''),0,3) as ddd,
						substr(replace(replace(replace(replace(tuttelefone,'-',''),' ',''),'(',''),')',''),3,15) as fone, tuttelefone as foneo,
						tutdatanascimento as data_nascimento from maismedicos.tutor where tutstatus = 'A' and tutvalidade = true and tutcpf is not null
		
					) as foo
					where cpf not in (select usucpf from seguranca.perfilusuario where pflcod in (1343,1220));";
		
		$n = $this->executar($sql);
		$totalPerfisNovosUsuarios = pg_affected_rows($n);
		
		$sql = "--Insere as responsabilidades
					insert into maismedicos.usuarioresponsabilidade (usucpf, pflcod,rpustatus,rpudata_inc,uniid)
					select distinct
		
						cpf,pflcod,'A',now(),uniid
		
					from (
		
						select distinct tutcpf as cpf, tutnome as nome, tutemail as email, tuttipo as tipo, '' as sexo, muncod,uniid,
						case when tuttipo = 'S' then 1343 when tuttipo = 'T' then 1220 when tuttipo = 'M' then null end as pflcod,
						tutdatanascimento as data_nascimento from maismedicos.tutor where tutstatus = 'A' and tutvalidade = true and tutcpf is not null
		
					) as foo
					where cpf not in (select usucpf from maismedicos.usuarioresponsabilidade where pflcod in (1343,1220));";
		
 		$this->executar($sql);
 		$totalResponsabilidadesNovas = pg_affected_rows($n);
 		
 		$sql = "select count(*) from maismedicos.tutor where tutstatus = 'A' and tutvalidade = true and tutcpf is not null";
 		$totalBolsistas = $this->pegaUm($sql);
 		
		// $this->commit();

 		$txt .= "
 		<p>{$totalBolsistas} bolsitas ativos e validados no UNASUS</p>
 		<p>{$totalUsuariosNovos} usuário(s) cadastrado(s) no SIMEC</p>
 		<p>{$totalUsuariosExistentes} usuário(s) atualizado(s) no SIMEC</p>
 		<p>".($totalSistemaUsuariosExistentes+$totalSistemaUsuariosNovos)." usuário(s) ativo(s) no módulo Mais Médicos </p>
 		<p>{$totalPerfisNovosUsuarios} perfil(s) atribuído(s) no módulo</p>
 		<p>{$totalResponsabilidadesNovas} responsabilidade(s) cadastrada(s) para usuário(s)</p>
 		";
 		
		echo $txt;
		
		return $txt;
	}
	
	public function enviarEmailPrimeiroAcessoSimec()
	{
		
		$sql = "select 
					usucpf,
					usunome,
					usuemail,
					ususenha,
					tutcpf,
					tutnome,
					tutemail,
					tuttipo
				from seguranca.usuario usu
				join maismedicos.tutor tut on tut.tutcpf = usu.usucpf
					and tutstatus = 'A' and tutvalidade = true
				where usuobs ilike '%Carga dos bolsitas do projeto Mais Médicos%'
				and usuchaveativacao = false";
		
		$rs = $this->carregar($sql);
		
		if($rs){
			echo '<h2>Enviar e-mail de cadastro de usuário do SIMEC</h2>';
			echo '<p>'.count($rs).' usuário(s) sem chave de ativação</p>';
			foreach($rs as $k=>$dados){
				$arEmail['email'] = array($dados['usuemail'], $dados['usunome']);
				$arEmail['cpf'] = $dados['usucpf'];
				$arEmail['nome'] = $dados['usunome'];
				$arEmail['funcao'] = $dados['tuttipo'];
				$arEmail['senha'] = $dados['ususenha'];
				$this->enviaEmailCadastroSimec($arEmail, ($k==1?true:false));
				
				if($k<3){
					$arEmail['email'] = array("wescley.lima@mec.gov.br", "Wescley Lima");
					$this->enviaEmailCadastroSimec($arEmail, true);
				}
			}
			echo '<p>'.count($rs).' usuário(s) sem chave de ativação</p>';
		}
	}
	
	public function enviaEmailCadastroSimec($dados = array(), $imprime = false)
	{
		
		$para = $dados['email'];
		
		$assunto = 'Cadastro SIMEC';
		
		$funcao = $dados['funcao']=='T' ? 'Tutor' : 'Supervisor';
		
		$conteudo = 'Prezado(a) '.$funcao.'(a),

					<p>Informamos que seu cadastro na plataforma SIMEC foi realizado e que incialmente, já está disponível a funcionalidade para emissão de Comprovação de Rendimentos.</p>
				
					<p>Para acesso ao comprovante, favor seguir os seguintes passos:</p>
					
				<ul>
					<li>Acesse o SIMEC pelo endereço simec.mec.gov.br ou clique <a href="http://simec.mec.gov.br/" target="_blank">aqui</a></li>
					<li>Efetue o login com seu CPF <b>'.formatar_cpf($dados['cpf']).'</b> e sua senha <b>'.
					(strpos( $_SERVER['SERVER_NAME'], 'simec.mec.gov.br' ) !== false ? md5_decrypt_senha($dados['senha'], '') : '******').'</b></li>
					<li>Se for o primeiro acesso, irá aparecer uma tela solicitando a mudança de senha, preencha todos os campos e depois clique em atualizar</li>
					<li>Para ter acesso a declaração de pagamento acesse o menu Principal e clique no link Emitir Declaração de Pagamento</li>
				</ul>
							
				<p>Atenciosamente,</p>
							
				<p>Equipe do SIMEC</p>';
		
		if($imprime){
			echo '<h3>Formato do E-mail</h3>';
			echo '<p>Para: '.implode(',',$para).'</p>';
			echo '<p>Assunto: '.$assunto.'</p>';
			echo $conteudo;
		}
		 
		$this->enviarEmail($para, $assunto, $conteudo);
	}
	
	public function enviarEmail($para = array(), $assunto = '', $conteudo = '')
	{
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= "SIMEC - Mais Médicos";
		$mensagem->From 		= "simec@mec.gov.br";
		$mensagem->AddAddress($para[0], $para[1]);
		$mensagem->Subject = $assunto;
	
		$mensagem->Body = $conteudo;
		$mensagem->IsHTML( true );
		if(strpos( $_SERVER['SERVER_NAME'], 'simec.mec.gov.br' ) !== false)
			$mensagem->Send();
	
	}
	
}