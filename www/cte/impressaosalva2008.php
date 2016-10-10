<?


// obtém o tempo inicial da execução
//$Tinicio = getmicrotime();

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";




class cls_classes_simec {
	var $mess_erro;
	var $ultimo_erro;
	function cls_classes_simec() {
		$this->mess_erro[101] = "Não foi possível estabelecer uma conexão com o servidor LDAP.";
		$this->mess_erro[102] = "Não foi possível autenticar no servidor LDAP.";
		$this->mess_erro[103] = "A pesquisa não retornou resultado.";
		$this->mess_erro[104] = "Não foi possível estabelecer uma conexão com o cls_banco de Dados.";
		$this->mess_erro[105] = "Falha na seleção da base de dados.";
		$this->mess_erro[106] = "Falha na execução da operação. Foi enviada uma mensagem para o Administrador.";
		$this->mess_erro[107] = "Usuário ou senha inválida!";
		$this->mess_erro[108] = "Ocorreu um erro durante a execução do Bind.";
		$this->mess_erro[200] = "Registro cancelado.";
	}
}
class cls_banco
{
	var $link;
	var $resultado;
	var $num_rows;
	var $ultimo_SQL;
	var $sql;
	var $oci_id;
	var $tentativas;
	function cls_banco()
	{	
		cls_classes_simec::cls_classes_simec();
		while((!$this->link) and ($this->tentativas < 5))
		{
			//                	dbg( "host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=".$GLOBALS['nome_bd']."  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."" );
			$this->link = pg_connect("host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=dbsimec_janeiro  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."");
			pg_query($this->link, "SET search_path TO seguranca,monitora,elabrev,public");
			// pg_query($this->link, "SET search_path TO public");

			pg_set_client_encoding($this->link,'LATIN5');
			// pg_set_client_encoding($this->link,'UTF-8');
			$this->tentativas++;
		}

		if((!$this->link) and ($this->tentativas == 5))
		cls_classes_simec::erro(104,2);
		else
		$this->tentativas = 0;
		return 1;
	}


	public function __destruct()
	{
		if (isset($_SESSION['transacao']))
		{
			pg_query($this->link, 'rollback; ');
			unset($_SESSION['transacao']);
		}
		if (is_resource($this->link)) {@pg_close( $this->link );}
	}




	///////////////////Grficos do sistema
	function grafico_validacao_prg($prgid=0,$refcod=0)
	{
		$sql= 'select * from validacaostat where refcod='.$refcod.' and prgid='.$prgid;
		$rs=$this->recuperar($sql);
		if (!is_array($rs))
		return '<img src="../imagens/valida3.gif" align="absmiddle" width="15" height="15">';
		else
		return monta_grafico_validacao($rs['faltap'],$rs['total_cor1'],$rs['total_cor2'],$rs['total_cor3'],$rs['acatotal']);

	}

	////////////////////Fim Grficos


	function eof($sql)
	{
		$RS = $this->carregar($sql);
		$nlinhas = $RS ? count($RS) : 0;
		if ($nlinhas==0) return true; else return false;
	}

/**
 * Função que altera uma string evitando a presença de caracteres próprios do sql
 *
 * @param unknown_type $string
 * @param unknown_type $p
 * @return string 
 */	
function antiInjection($string,$p="")
{
	// remove palavras que contenham sintaxe sql
	$string = str_replace('"',"\'",$string);
	$string = str_replace("'","\'",$string);

	$string = ($string);
	if($p !=2){
		$string = str_replace( "|" , "" , $string);
	}
	$string = preg_replace(sql_regcase("/(from|select|insert|delete|where|drop table|show tables|#|--|\\\\)/"),"",$string);
	$string = trim($string);//limpa espaços vazio
	//	$string = strip_tags($string);//tira tags html e php
	$string = addslashes($string);//Adiciona barras invertidas a uma string
	if($p!=1){
		if(strlen(trim($string))==0) $string ='';
	}

	return $string;

}

	/**
	 *
	 */
	function executar( $SQL, $auditoria = true )
	{
		if (!isset($_SESSION) || !array_key_exists('usucpforigem', $_SESSION) || $_SESSION['usucpforigem'] == '') {
			return false;
		}

		$_SESSION['sql'] = $SQL;
		if (!isset($_SESSION['transacao'])) {
			$this->resultado = pg_query($this->link, 'begin transaction; ');
			$_SESSION['transacao'] = '1';
		}

		$this->resultado = @pg_query($this->link, $SQL);
		if ( $this->resultado == null )
		throw new Exception( $SQL . pg_errormessage( $this->link ) );

		if ( $auditoria ) {
			//Inicio - Gravando na tabela de auditoria
			// detecta operacao e tabela (Insert, Update ou Delete)
			$audtipo = strtoupper(substr(trim($SQL),0,1));
			//detecta qual tabela foi gravado
			if ($audtipo == 'I') $audtabela = substr(trim($SQL),strpos(trim(strtoupper($SQL)),' INTO ')+6, strlen($SQL));
			elseif ($audtipo == 'U') $audtabela = substr(trim($SQL),strpos(trim(strtoupper($SQL)),'UPDATE ')+7, strlen($SQL));
			elseif ($audtipo == 'D') $audtabela = substr(trim($SQL),strpos(trim(strtoupper($SQL)),' FROM ')+6, strlen($SQL));
			else $audtabela = 'X';
			if ($audtabela<>'X') $audtabela = substr(trim($audtabela),0,strpos($audtabela,chr(32)));
			//Se no tiver vindo de algum módulo
			if ($_SESSION['mnuid']=='') $_SESSION['mnuid']=1;
			//Insere dados na tabela auditoria
			if (!$_SESSION['sisid'])
			$id=4;
			else
			$id = $_SESSION['sisid'];

			$sql_audit = "insert into auditoria (usucpf, mnuid, audsql, auddata, audtabela, audtipo, audip, sisid) values ('".$_SESSION['usucpforigem']."', ".$_SESSION['mnuid'].", '".str_replace("'","''",stripslashes($SQL))."', '".date('Y-m-d H:i:s')."', '".$audtabela."', '".$audtipo."', '".$_SERVER["REMOTE_ADDR"]."',$id)";
			pg_query($this->link, $sql_audit);
		}

		return $this->resultado;
	}
	

	function apagatudo($texto,$tabela) {
		$this->resultado = pg_query($this->link, 'delete from '.$tabela);
		return $this->resultado;
	}
	function recuperar($SQL, $var = null) {
		if (! $SQL or $SQL=='')
		return null;
		else {
			$_SESSION['sql'] = $SQL;

			$res = pg_fetch_all(pg_query($this->link, $SQL));
			if ( $var != null )
			{
				global ${$var};
				${$var} = $res[0];
			}
			return $res[0];
		}
	}


	function busca_explicacao($campo,$tabela)
	{
		$sql="select p2.atttypmod-4 as tmax, p4.typname as tipo from pg_attribute p2 left join pg_type p4 on p2.atttypid=p4.oid left join pg_description p1 on p2.attnum=p1.objsubid left join pg_class p3 on p3.relfilenode=p2.attrelid and p3.relname='$tabela' where p2.attname='$campo' limit 1";

		$RS = $this->record_set($sql);
		$res = @$this->carrega_registro($RS,0);
		if ($res['explicacao']=='')
		{

			$sql="select distinct p1.description as explicacao from pg_description p1 inner join pg_attribute p2 on p2.attrelid=p1.objoid and trim(p2.attname) = '$campo' and p2.attnum=p1.objsubid inner join pg_class p3 on p3.relname='$tabela' and p3.reltype::integer=p2.attrelid::integer +1 ";


			$RS = $this->record_set($sql);
			$nlinhas = $this->conta_linhas($RS);

			if ($nlinhas>=0)
			{
				$res2 = @$this->carrega_registro($RS,0);
				$res['explicacao'] = $res2['explicacao'];

			} else   $res['explicacao'] = 'Campo sem descrição detalhada';
		}

		return $res;

	}

	function carregar($SQL, $var = null) {

		if( $SQL == null )
		{
			//				dbg( debug_backtrace() );
		}
		$_SESSION['sql'] = $SQL;
		$res = pg_fetch_all(pg_query($this->link, $SQL));

		if( $res === null )
		{
			throw new Exception( pg_last_error( $this->link ) );
		}
		if ( $var != null )
		{
			global ${$var};
			${$var} = $res;
		}
		return $res;
	}

	function carregarColuna($SQL, $coluna = '',$var = null) {

		if( $SQL == null )
		{
			//				dbg( debug_backtrace() );
		}
		$_SESSION['sql'] = $SQL;
		$res = pg_fetch_all(pg_query($this->link, $SQL));

		if( $res === null )
		{
			throw new Exception( pg_last_error( $this->link ) );
		}
		if ( $var != null )
		{
			global ${$var};
			${$var} = $res;
		}

		if(!$res) return array();

		$result = array();

		foreach ($res as  $row ){
			if(!$coluna){
				array_push($result,reset($row));
			}
			else{
				array_push($result,$row[$coluna]);
			}
		}
		return $result;
	}


	function verifica_momento() {
		// esta função verifica o momento
		$sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento1 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
		if ($this->pegaUm($sql)) return 1;
		else {
			$sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento2 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
			if ($this->pegaUm($sql)) return 2;
			else return 0;
		}
	}

	// $db->ehdecisor($_SESSION['acaid'],'A',$momento);
	function ehcoorduma($cpf)
	{
		// esta função verifica se o usuário possui perfil de coordenador da uma
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='$cpf'";
		if ($this->pegaum($sql)) return 1; else return 0;

	}
	function ehdecisor($cod,$tab,$mom)
	{
		// esta função verifica, pelo cpf do usuário logado e pelo momento se ele é decisor externo, interno ou não é decisor
		if ($this->testa_superuser()) return 1;
		if ($mom==1)
		{
			if ($tab=='A')
			{
				// ação no momento 1
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,67) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de coordenador de planejamento de unidade ou ação
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em ação
						$sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
			if ($tab=='P')
			{
				// programa no momento 1
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,67) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de coordenador de planejamento de unidade ou ação
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em programa
						$sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
		}
		if ($mom==2)
		{
			if ($tab=='A')
			{
				// ação no momento 2
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql)) return 1; // coordenador da UMA

				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de uma de unidade ou ação ou coord uma
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em ação
						$sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
			if ($tab=='P')
			{
				// programa no momento 2
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql)) return 1; // coordenador da UMA

				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de uma de unidade ou ação ou coord uma
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em ação
						$sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
		}
	}

	function ehdigitador($cod,$tab,$mom)
	{

		// esta função verifica, pelo cpf do usuário logado e pelo momento se ele é digitador externo, interno ou não é digitador
		if ($mom==1)
		{
			if ($tab=='A')
			{
				// ação no momento 1
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (33,68) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de equipe de planejamento de unidade ou ação
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em ação
						$sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
			if ($tab=='P')
			{
				// programa no momento 1
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (33,68) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de equipe de planejamento de unidade ou ação
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em programa
						$sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
		}
		if ($mom==2)
		{
			if ($tab=='A')
			{
				// ação no momento 2
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36,62) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql)) return 1; // é coordenador da UMA ou super usuário

				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de UMA unidade ou programa
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em ações no programa específico
						$sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid in (select prgid from elabrev.ppaacao_proposta where acaid=$cod) and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
			if ($tab=='P')
			{
				// programa no momento 2
				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36,62) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql)) return 1; // é coordenador da UMA ou super usuário

				$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
				if ($this->pegaum($sql))
				{
					// então o usuário possui perfil de equipe de perfil de UMA unidade ou programa
					// testo a unidade
					$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					if ($this->pegaUm($sql)) return 1;
					else
					{
						// testo a responsabilidade em programa
						$sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						if ($this->pegaUm($sql)) return 1;
						else return 0;
					}
				}

			}
		}
	}

	/**
	 * @param string $texto
	 * @return string
	 */
	function escape( $mixValorCampo  ){


		if( ( gettype( $mixValorCampo ) == 'integer' ) || ( gettype( $mixValorCampo ) == 'float') )
		{
			return $mixValorCampo;
		}
		if( is_string( $mixValorCampo ) )
		{
			return "'" . pg_escape_string( $this->link, $mixValorCampo ) . "'" ;
		}
		if( is_null( $mixValorCampo ) )
		{
			return 'NULL';
		}
		return $mixValorCampo;
	}

	function verifica_momento2() {
		// esta função verifica, pelo cpf do usuário logado e pela data se ele tem direito de agir no sistema e se está dentro do seu momento.
		//1. verificando se o usuário é coordenador de planejamento ou equipe de apoio, ou superusuário ou UMA ou coordenador da UMA
		$retorna=0;
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (33,68) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp > 0 ) {$ok=1;$respeqpplan = $resp;}
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,67) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp > 0 ) {$ok=1;$respcoordplan = $resp;}
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp > 0 ) {$ok=1;$respuma = $resp;}
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp > 0 ) {$ok=1;$respcoorduma = $resp;}
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (62) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = $this->pegaUm($sql);
		if ($resp > 0 ) {$ok=1;$respsupusu = $resp;	}

		if ($ok > 0)
		{
			// então pode editar
			// verifica se a data está dentro do período autorizado
			// $sql="select refdata_inicio as inicio, refdata_limite_momento1 as momento1,refdata_limite_momento2 as momento2,refdata_limite_momento3 as momento3 from elabrev.referencia where refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
			//$momentos=$this->pegaLinha($sql,0);
			// se for coordenador de planejamento ou equipe de planejamento - perfil 24 ou 33 e se estiver dentro do momento 1, então ok.
			if ($respcoordplan or $respeqpplan or $respsupusu)
			{
				$sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento1 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
				$ref=$this->pegaUm($sql) ;
			}
			// se for uma ou coordenador da uma e se estiver dentro do momento 2, então ok.
			if ($respcoorduma or $respuma or $respsupusu)
			{
				$sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento2 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
				$ref=$this->pegaUm($sql) ;
			}
			if ($ref) $retorna=1;

		}
		return $retorna;
	}

	function ehcriador($cod,$busca=0)
	{
		if (! $busca or $busca=='A')
		$sql = "select acaid from elabrev.ppaacao_proposta where usucpf='".$_SESSION['usucpf']."' and acaid=$cod and prsano='".$_SESSION['exercicio']."'";
		else if ($busca == 'P')
		$sql = "select prgid from elabrev.ppaprograma_proposta where usucpf='".$_SESSION['usucpf']."' and prgid=$cod and prsano='".$_SESSION['exercicio']."'";
		else if ($busca == 'EA')
		$sql = "select acaid from elabrev.proposta_exclusao_acao where usucpf='".$_SESSION['usucpf']."' and acaid=$cod and prsano='".$_SESSION['exercicio']."'";
		else if ($busca == 'MA')
		$sql = "select acaid from elabrev.proposta_migracao_acao where usucpf='".$_SESSION['usucpf']."' and acaid=$cod and prsano='".$_SESSION['exercicio']."'";
		else if ($busca == 'EP')
		$sql = "select prgid from elabrev.proposta_exclusao_programa where usucpf='".$_SESSION['usucpf']."' and prgid=$cod and prsano='".$_SESSION['exercicio']."'";
		else if ($busca == 'CP')
		$sql = "select eraid from elabrev.elaboracaorevisao where usucpf='".$_SESSION['usucpf']."' and eraid=$cod and prsano='".$_SESSION['exercicio']."'";

		if ($this->pegaUm($sql)) return true;
		else return false;
	}

	function verifica_resp_elabrev($cod=0,$tab=0)
	{
		unset($_SESSION['uniorc']);
		//$cod= ação ou programa
		//$tab = tabela de ação ou tabela de programa
		// esta função verifica, pelo cpf do usuário logado e pela id da ação e do programa se ele pode editar o registro.
		//1. verificando se o usuário é coordenador de planejamento ou equipe de apoio, ou superusuário ou UMA ou coordenador da UMA
		$retorna=0;
		$sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,33,36,62,63,66,67,68) and pu.usucpf='".$_SESSION['usucpf']."'";
		$resp = @$this->carregar( $sql );
		if (  $resp && count($resp) > 0 )
		{
			foreach ( $resp as $linha )
			{
				foreach($linha as $k=>$v) ${$k}=$v;
				if ($pflcod == 24 or $pflcod==33)
				{
					// então pode editar
					//verifica se a ação pertence à unidade
					if ($_SESSION['acaid'])
					{
						$sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur
inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod
inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=".$_SESSION['acaid']." where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						$ret1=$this->pegaUm($sql);
					}
					//$retorna=$resp;
					// verifica, se for perfil 24 ou 33 quais as unidades que ele pode operar
					$sql= "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
					$rs = $this->carregar($sql);
					if (  $rs && count($rs) > 0 )
					{
						foreach ( $rs as $ln )
						{
							foreach($ln as $k=>$v) ${$k}=$v;
							$_SESSION['uniorc'][] = $unicod;
						}
					}
				}
				if ($pflcod == 67 or $pflcod==68)
				{
					// então pode editar
					//verifica se está responsável pela ação
					if ($_SESSION['acaid'])
					{
						$sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=".$_SESSION['acaid']." and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						$ret2=$this->pegaUm($sql);
					}
				}
				if ($pflcod == 67 or $pflcod==68)
				{
					// então pode editar
					// verifica se está responsável pela ação
					if ($_SESSION['acaid'])
					{
						$sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=".$_SESSION['acaid']." and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
						$ret3=$this->pegaUm($sql);
					}
				}
				if ($pflcod == 62 or $pflcod==34 or $pflcod==36)
				{
					unset($_SESSION['uniorc']);
					$ret4 = 1;
				}
			}
		}
		if ($ret1) $retorna=$ret1;
		if ($ret2) $retorna=$ret2;
		if ($ret3) $retorna=$ret3;
		if ($ret4) $retorna=$ret4;

		return $retorna;
	}


	function verifica_geral()
	{
		// esta funação verifica vários itens
		// verificando o plano de trabalho
		// fase 1 - verifica se o usuário tem responsabilidades em ação
		$sql="select ur.acaid from monitora.usuarioresponsabilidade ur inner join seguranca.usuario u on u.usucpf=ur.usucpf inner join monitora.acao a on a.acaid=ur.acaid and a.prgano='".$_SESSION['exercicio']."' where ur.acaid is not null and ur.usucpf='".$_SESSION['usucpf']."'";

		$RS = $this->record_set($sql);
		$nlinhas = $this->conta_linhas($RS);
		if ($nlinhas >= 0) {
			// então o usuário possui responsabilidade
			for ($i=0;$i<=$nlinhas;$i++)
			{
				$res =  $this->carrega_registro($RS,$i);
				if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
				// para cada ação, verifico se existe plano de trabalho
				$sql = "select p.ptoavisoantecedencia  as antec from monitora.plantrabacao pa inner join monitora.planotrabalho p on p.ptoid=pa.ptoid and p.ptotipo in ('F','E') and to_char(p.ptodata_fim,'YYYY') >='".date('Y')."' "." where pa.acaid=$acaid";
				$res =  $this->pegaUm($sql);
				if ($res) $antec=$res;
				if (! $antec) $antec=7;
				$sql = "select pa.* , p.ptodata_ini,p.ptodata_fim, p.ptodsc,a.acacod, a.acadsc from monitora.plantrabacao pa inner join monitora.planotrabalho p on p.ptoid=pa.ptoid and p.ptotipo in ('F','E') and ptodata_ini <= current_date- interval '$antec days' and to_char(p.ptodata_fim,'YYYY') >='".date('Y')."' "." inner join acao a on a.acaid=pa.acaid where pa.acaid=$acaid";

				$RS2 = $this->record_set($sql);
				$nlinhas2 = $this->conta_linhas($RS2);
				// agora tenho os planos de trabalho da ação testada.
				// para cada ptoid vou verificar se já foi lançado algum evento.
				$ok=1;
				if ($nlinhas2 >= 0) {
					for ($ii=0;$ii<=$nlinhas2;$ii++)
					{
						$res =  $this->carrega_registro($RS2,$ii);
						$ok=0;
						if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
						$sql = "select e.expid from monitora.execucaopto e where ptoid = $ptoid and acaid = $acaid";
						$RS3 = $this->record_set($sql);
						$nlinhas3 = $this->conta_linhas($RS3);
						if ($nlinhas3 >= 0) {$ok=1;break;}
						else
						{
							// então não fez nada que devia
							$_SESSION['texto'][] ="Você cadastrou atividades no Plano de Trabalho - $ptodsc da ação $acacod-$acadsc e até o momento não monitorou a atividade";
							$popup=1;
						}
					}
				}

			}
			if ($popup)
			{
				?>
<script>       	     	
             	     	e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/msg_geral&acao=A";
             	     	window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=600,height=400'");
             	     	</script>
				<?
}
}
}

function monta_combo($var,$sql,$habil,$titulo='',$acao,$opc,$txtdica='',$size='',$obrig='', $id = '', $return = false)
{
	// este método monta um combobox onde: var = nome do combo; sql é a query,
	//titulo é o título da primeira linha do combo
	global ${$var};
    if (is_array($sql))
        $rescombo = $sql;
    else
	    $rescombo = $this->carregar( $sql );

	$select = "";
	if ( $txtdica )
	$select = "<span onmouseover=\"return escape('". $txtdica . "');\">";

	// define atributos do select
	if ( $habil == 'S' )
	$select .= "<select name='" . strtolower( $var ) . "' ";
	else
	$select .= "<select name='" . strtolower( $var ) . "_disable' ";
	$select .= " class='CampoEstilo' ";

	if ( $size <> '' )
	$select .= " style='width:".$size."px;'";
	if ( $habil == 'N' )
	$select = $select. ' disabled="disabled" ';
	if ( isset( $tamanho ) && $tamanho > 1 )
	$select .= ' size="' . $tamanho . '" ';
	if ( $acao )
		$select .= ' onchange="' . $acao . '(this.value)"';
	if ( $id )
	$select .= ' id="' . simec_htmlentities( $id ).  '"';
	$select .= ">";

	// cria os options
	if ( $titulo )
	$select .= '<option value="">'.$titulo.'</option>';

	if($rescombo){
		for ( $i = 0; $i < count( $rescombo ); $i++ )
		{
			if ( ${$var} == $rescombo[$i]['codigo'] )
			$sel = 'selected="selected"';
			else
			$sel='';
			$select .= "<option value='" . $rescombo[$i]['codigo'] . "' " . $sel . ">" .
			$rescombo[$i]['descricao'] .
			"</option>\n";
		}
	}
	if ( ${$var} == 'x')
	$sel = 'selected="selected"';
	else
	$sel='';
	if ( $opc )
	$select .= "<option " . $sel . " value='x'>" . $opc . "</option>\n";

	// finaliza impressão do select
	$select .= '</select>';
	if ( $txtdica )
	$select .= "</span>";
	if ( $habil == 'N' )
	$select .= "<input type='hidden' name ='" . strtolower( $var ) . "' value='" . ${$var} . "' />";
	if ( $obrig == 'S' )
	$select .= obrigatorio();

	if ($return)
	return $select;
	else
	echo $select;
}


function monta_combo_multiplo($var,$sql,$habil,$titulo='',$acao,$opc,$txtdica='', $tamanho=4) {
	// este mtodo monta um combobox onde: var = nome do combo; sql  a query,
	//titulo  o ttulo da primeira linha do combo
	global ${$var};

	if(!is_array(${$var}) || @count(${$var})<1) {
		${$var} = array();
	}

	$rescombo=$this->carregar($sql);

	$select = "";
	if ($txtdica) $select = "<span onmouseover=\"return escape('$txtdica');\">";
	$select .= "<select name='".strtolower($var)."[]' class='CampoEstilo' size='" . $tamanho . "' multiple='multiple'";
	if ($habil=='N')      $select = $select. ' disabled="disabled" ';

	if ($acao) $select = $select. ' onChange="'.$acao.'(this.value)"';
	$select = $select .">";
	print $select;

	if($titulo) print '<option ' . ((!isset(${$var}) || (@count(${$var})==1 && !(bool)${$var}[0]) ) ? 'selected' : ''). ' value="">'.$titulo.'</option>';
	for ($i=0;$i<count($rescombo);$i++) {
		if (array_search($rescombo[$i]['codigo'], ${$var}) !== false) $sel = '  SELECTED'; else $sel='';
		print "<option ".$sel." value='".$rescombo[$i]['codigo']."'>".$rescombo[$i]['descricao']."</option>";
	}
	if (array_search('x', ${$var}) !== false) $sel = '  SELECTED'; else $sel='';
	if ($opc and $opc<>'') print "<option ".$sel." value='x'>".$opc."</option>";
	print '</select>';
	if ($txtdica) print "</span>";
	if ($habil=='N') print "<input type='hidden' name ='".strtolower($var)."' value='".${$var}."' />";

}

/*
 * Monta checkboxes a partir de um select as colunas codigo e descricao
 *
 * @param $nome String nome do campo
 * @param $query String a consulta sql que vai ser executada
 * @param $marcados Array traz os itens que devem ser selecinados
 * @param $separador String elemento a ser usado entre dois checkboxes por padrão   
 */
function monta_checkbox($nome, $query, $marcados, $separador='  ') {
	if(!is_array($marcados) || @count($marcados)<1) {
		$marcados = array();
	}

	$saida = "";
	$rescombo=$this->carregar($query);
	if(is_array($rescombo) && @count($rescombo)>0) {
		for ($i=0, $j=count($rescombo)-1;$i<count($rescombo);$i++) {
			$checked = array_search($rescombo[$i]['codigo'], $marcados) !== false ? 'checked="checked"' : '';
			echo "<input type=\"checkbox\" name=\"" . $nome . "\" value=\"" . $rescombo[$i]['codigo'] . "\" " . $checked . " /> " . $rescombo[$i]['descricao'];
			if($i!=$j) echo $separador;
		}
	}
}

function cria_aba($abacod_tela,$url,$parametros)
{
	//Função cria aba que monta as abas visualmente
	if (trim($abacod_tela)<>'')
	{
		$sql = "select menu.mnuid, menu.mnudsc, menu.mnulink, menu.mnutransacao from seguranca.menu, seguranca.aba_menu where menu.mnuid=aba_menu.mnuid and aba_menu.abacod=".$abacod_tela." and menu.mnuid in(select distinct m2.mnuid from perfilmenu m2, perfilusuario p where m2.pflcod=p.pflcod and p.usucpf='".$_SESSION['usucpf']."') order by menu.mnucod";
		$RS = $this->carregar($sql);
		if(is_array($RS))
		{
			print '<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint"><tr><td><table cellpadding="0" cellspacing="0" align="left"><tr>';
			$nlinhas = count($RS)-1;
			for ($j=0; $j<=$nlinhas;$j++)
			{
				foreach($RS[$j] as $k=>$v) ${$k}=$v;
				if ($url<>$mnulink and $j==0) $gifaba = "aba_nosel_ini.gif";
				elseif ($url==$mnulink and $j==0) $gifaba = "aba_esq_sel_ini.gif";
				elseif ($gifaba=='aba_esq_sel_ini.gif' or $gifaba=='aba_esq_sel.gif') $gifaba = "aba_dir_sel.gif";
				elseif ($url<>$mnulink) $gifaba = "aba_nosel.gif";
				elseif ($url==$mnulink) $gifaba = "aba_esq_sel.gif";
				$parametro = is_array( $parametros ) ? $parametros[$j] : $parametros;
				if ($url==$mnulink) {$giffundo_aba = "aba_fundo_sel.gif";$cor_fonteaba="#000055";} else {$giffundo_aba = "aba_fundo_nosel.gif";$cor_fonteaba="#4488cc";}
				print '<td height="20" valign="top"><img src="../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td>';
				print '<td height="20" align="center" valign="middle" background="../imagens/'.$giffundo_aba.'" style="color:'.$cor_fonteaba.'; padding-left: 10px; padding-right: 10px;">';
				if ($mnulink<>$url){print '<a  href="'.$mnulink.$parametro.'" style="color:'.$cor_fonteaba.';" title="'.$mnutransacao.'">'.$mnudsc.'</a>';} else {print $mnudsc.'</td>';}
			}
			if ($gifaba=='aba_esq_sel_ini.gif' or $gifaba=='aba_esq_sel.gif') $gifaba= "aba_dir_sel_fim.gif"; else $gifaba = "aba_nosel_fim.gif";
			print '<td height="20" valign="top"><img src="../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td></tr></table></td></tr></table>';
		}
	}
}

/*
 * Monta um arquivo Excel a partir de uma Query
 *
 * @param $sql String Query a ser executada
 * @param $arquivo String prefixo do arquivo a ser gerado
 * @param $cabecalho Array Opcional, nome dos campos no cabecalho do arquivo
 * @param $formatocoluna Array pode ser n (Numero) ou s (String)
 */
function sql_to_excel($sql,$arquivo,$cabecalho="",$formatocoluna="") {
	// este método transforma uma query em excel
	global $nomeDoArquivoXls;
	$nomeDoArquivoXls = "SIMEC_".date("His")."_".$arquivo;

	$xls = new GeraExcel();
	$RS=$this->carregar($sql);
	$nlinhas = $RS ? count($RS) : 0;
	if (! $RS) $nl = 0; else $nl=$nlinhas;
	if ($nlinhas>0)
	{
		//Monta Cabeçalho
		if(is_array($cabecalho))
		{
			for ($i=0;$i<count($cabecalho);$i++)
			{
				$xls->MontaConteudoString(0, $i, $cabecalho[$i]);
			}
		}
		else
		{
			$col=0;
			$lin=0;
			foreach($RS[0] as $k=>$v)
			{
				$xls->MontaConteudoString($lin, $col, $k);
				$col++;
			}
		}
		//Monta Listagem
		for ($i=0;$i<$nlinhas;$i++)
		{
			$lin = $i+1;
			$col = 0;
			foreach($RS[$i] as $k=>$v)
			{
				if ($formatocoluna[$col]=='n')
				$xls->MontaConteudoNumero($lin, $col, $v);
				else
				$xls->MontaConteudoString($lin, $col, $v);
				$col++;
			}
		}
		$xls->GeraArquivo();
	}
}


function monta_lista_simples($sql,$cabecalho="",$perpage,$pages,$soma='N',$largura='95%') {
	if(!(bool)$largura) $largura = '95%';
	// este método monta uma listagem na tela baseado na sql passada
	//Registro Atual (instanciado na chamada)
	if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);

    if (is_array($sql))
        $RS = $sql;
    else
        $RS = $this->carregar($sql);

	$nlinhas = $RS ? count($RS) : 0;
	if (! $RS) $nl = 0; else $nl=$nlinhas;
	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
	print '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
	if ($nlinhas>0)
	{
		//Monta Cabeçalho
		if(is_array($cabecalho))
		{
			print '<thead><tr>';
			for ($i=0;$i<count($cabecalho);$i++)
			{
				print '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].'</label>';
			}
			print '</tr> </thead>';
		}

        echo '<tbody>';

		//Monta Listagem
		$totais = array();
		$tipovl = array();
		for ($i=($numero-1);$i<$reg_fim;$i++)
		{
			$c = 0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
			foreach($RS[$i] as $k=>$v) {

				if (is_numeric($v))
				{
					//cria o array totalizador
					if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
					//Mostra o resultado
					if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
					if ($v<0) print '<td align="right" style="color:#cc0000;" title="'.$cabecalho[$c].'">('.$v.')'; else print '<td align="right" style="color:#999999;" title="'.$cabecalho[$c].'">'.$v;
					print ('<br>'.$totais[$c]);
				}
				else print '<td title="'.$cabecalho[$c].'">'.$v;
				print '</td>';
				$c = $c + 1;
			}
			print '</tr>';
		}

        print '</tbody>';

		$somarCampos = $soma!='S' && is_array($soma) && (@count($soma)>0);
		if ($soma=='S' || $somarCampos){
			//totaliza (imprime totais dos campos numericos)
			print '<tfoot><tr>';
			for ($i=0;$i<$c;$i++)
			{
				print '<td align="right" title="'.$cabecalho[$i].'">';

				if ($i==0) print 'Totais:   ';
				if(($somarCampos && $soma[$i]) || $soma=='S') {
					if (is_numeric($totais['0'.$i])) print number_format($totais['0'.$i], 2, ',', '.'); else print $totais['0'.$i];
				}
				print '</td>';
			}
			print '</tr></tfoot>';
			//fim totais
		}

	}
	else {
		print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
	}
	print '</table>';
}

/**
 * Monta uma lista com sumário a partir de um array multidimensional
 *
 */
function monta_lista_agrupado($arrRsAgrupado, $cabecalho="",$soma='',$largura='95%') {
	if(!(bool)$largura) $largura = '95%';

	$somarCampos = $soma!='' && is_array($soma) && (@count($soma)>0);
	$nlinhas = $arrRsAgrupado ? count($arrRsAgrupado) : 0;

	print '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
	if ($nlinhas>0)
	{
		// Monta Cabeçalho
		if(is_array($cabecalho))
		{
			print '<thead><tr>';
			foreach($cabecalho as $cab) {
				print '<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cab."</td>";
			}
			print '</tr></thead>';
		}

		// Monta Listagem
		$totais = array();
		foreach($arrRsAgrupado as $grupo=>$dados) {
			foreach ($dados as $pos=>$linha) {
				$marcado = $pos % 2 ? '' : '#F7F7F7';
				echo '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
				$i=0;
				foreach($linha as $col=>$val) {
					if(!isset($totais[$grupo][$col]))
					$totais[$grupo][$col] = 0;

					if (!is_numeric($val)) {
						print '<td title="'.$cabecalho[$i].'">'.$val.'</td>';
					}
					else {
						$totais[$grupo][$col] += $val;
						$valTela = $somarCampos && $soma[$i] ? number_format($val, 2, ',', '.') : $val;

						if ($val<0) {
							echo '<td align="right" style="color:#cc0000;" title="'.$cabecalho[$i].'">(' . $valTela . ')</td>';
						}
						else {
							echo '<td align="right" style="color:#999999;" title="'.$cabecalho[$i].'">'. $valTela .'</td>';
						}
					}
					$i++;
				}
				echo "</tr>\n";
			}
			//
			// Subtotal
			if ($somarCampos) {
				print '<thead><tr>';
				$i=0;
				foreach ($totais[$grupo] as $col=>$total) {
					print '<td align="right" title="'.$cabecalho[$i].'">';
					if ($i==0) print 'Subtotal:   ';
					if ($somarCampos && $soma[$i]) {
						if (is_numeric($total))
						print number_format($total, 2, ',', '.');
						else
						print $total;
					}
					else {
						print " ";
					}
					print '</td>';
					$i++;
				}
				print '</tr></thead>';
			}
		}
		//
		// Total geral
		if ($somarCampos) {
			$totalGeral = array();
			foreach ($totais as $grupo=>$colunas) {
				foreach ($colunas as $col=>$total) {
					$totalGeral[$col] += $total;
				}
			}

			$i=0;
			print '<tr><td colspan="' . count($cabecalho) . '"> </td></tr>';
			print '<thead><tr>';

			foreach($totalGeral as $col=>$total) {
				print '<td align="right" title="'.$cabecalho[$i].'">';
				if ($i==0) print '<b>Total geral:</b>   ';
				if ($somarCampos && $soma[$i]) {
					if (is_numeric($total))
					print number_format($total, 2, ',', '.');
					else
					print $total;
				}
				else {
					print " ";
				}
				print '</td>';
				$i++;
			}
			print '</tr></thead>';
		}
	}
	else {
		print '<tr><td align="center" style="color:#cc0000;">não foram encontrados registros.</td></tr>';
	}
	print '</table>';
}

function busca_pai($cod,$i)
{
	// busca, num plano de trabalho, se o registro está subordinado
	$sql = "select ptoid_pai from planotrabalho where ptostatus='A' and ptoid=".$cod;
	//print $sql;
	//exit();
	$RS = $this->record_set($sql);
	$res = $this->carrega_registro($RS,0);
	if ($res['ptoid_pai']<>'')
	return 2;
	else
	{
		// verifico se tem filho alm de no ter pai
		$sql = "select ptoid from planotrabalho where ptostatus='A' and ptoid_pai=".$cod;
		$RS = $this->record_set($sql);
		$nlinhas = $this->conta_linhas($RS);
		if ($nlinhas < 0)
		return 1;
		else return 0;
	}

}


function monta_lista($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$par2) {
	// este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
	//$sql = Texto - sql que vai gerar a lista
	//$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
	//Parmetros de paginao
	//$perpage = Numrico - Registros por pgina
	//$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
	// $soma = Boleano - Mostra somatrio de campos numricos no fim da lista
	// $ordem = alinhamento dos títulos (left, rigth, center)
	// $par2 = Reservado para o futuro
	//Registro Atual (instanciado na chamada)
	if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);
	//Controla o Order by
	if (!is_array($sql) && $_REQUEST['ordemlista']<>'')
	{
		if ($_REQUEST['ordemlistadir'] <> 'DESC') {$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';} else {$ordemlistadir = 'DESC'; $ordemlistadir2 = 'ASC';}
		$subsql = substr($sql,0,strpos(trim(strtoupper($sql)),'ORDER '));
		$sql = (!$subsql ? $sql : $subsql).' order by '.$_REQUEST['ordemlista'].' '.$ordemlistadir;
	}

    if (is_array($sql))
        $RS = $sql;
    else
        $RS = $this->carregar($sql);

	$nlinhas = count($RS);
	if (! $RS) $nl = 0; else $nl=$nlinhas;
	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
	if ($nl>0)
	{
		$total_reg = $nlinhas;
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		//monta o formulario da lista mantendo os parametros atuais da pgina
		print '<form name="formlista" method="post"><input type="Hidden" name="numero" value="" /><input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/><input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';
		foreach($_POST as $k=>$v){if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero') print '<input type="Hidden" name="'.$k.'" value="'.$v.'"/>';}
		print '</form>';
		//Monta Cabealho
		if ( $cabecalho === null ) {

		}else if(is_array($cabecalho))
		{
			print '<thead><tr>';
			for ($i=0;$i<count($cabecalho);$i++)
			{
				if ($_REQUEST['ordemlista'] == ($i+1)) {
					$ordemlistadirnova = $ordemlistadir2;
					$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
				} else {
					$ordemlistadirnova = 'ASC';
					$imgordem = '';
				}
				print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$cabecalho[$i].'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
			}
			print '</tr> </thead>';
		}
		else
		{
			print '<thead><tr>'; $i=0;
			foreach($RS[0] as $k=>$v)
			{
				if ($_REQUEST['ordemlista'] == ($i+1)) {
					$ordemlistadirnova = $ordemlistadir2;
					$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
				} else {
					$ordemlistadirnova = 'ASC';
					$imgordem = '';}
					print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$k.'">'.$imgordem.'<strong>'.$k.'</strong></label>';
					$i=$i+1;}
					print '</tr> </thead>';
		}
		//Monta Listagem
		$totais = array();
		$tipovl = array();
		for ($i=($numero-1);$i<$reg_fim;$i++)
		{
			$c = 0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
			foreach($RS[$i] as $k=>$v) {
				if (is_numeric($v))
				{
					//cria o array totalizador
					if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
					//Mostra o resultado
					if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
					if ($v<0) print '<td align="right" style="color:#cc0000;" title="'.$cabecalho[$c].'">('.$v.')'; else print '<td align="right" style="color:#0066cc;" title="'.$cabecalho[$c].'">'.$v;
					print ('<br>'.$totais[$c]);
				}
				else print '<td title="'.$cabecalho[$c].'">'.$v;
				print '</td>';
				$c = $c + 1;
			}
			print '</tr>';
		}

		if ($soma=='S'){
			//totaliza (imprime totais dos campos numericos)
			print '<thead><tr>';
			for ($i=0;$i<$c;$i++)
			{
				print '<td align="right" title="'.$cabecalho[$i].'">';

				if ($i==0) print 'Totais:   ';
				if (is_numeric($totais['0'.$i])) print number_format($totais['0'.$i], 2, ',', '.'); else print $totais['0'.$i];
				print '</td>';
			}
			print '</tr>';
			//fim totais
		}

		print '</table>';
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: '.$nl.'</b></td><td>';

		include APPRAIZ."includes/paginacao.inc";
		print '</td></tr></table>';
		print '<script language="JavaScript">function ordena(ordem, direcao) {document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} function pagina(numero) {document.formlista.numero.value=numero;document.formlista.submit();}</script>';
	}
	else
	{
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
		print '</table>';
	}
}

function monta_radio($var,$sql,$habil,$op) {
	// este mtodo monta uma sequencia html radio onde:
	// var = Texto - Nome do campo Radio;
	// sql  = Texto - Consulta para gerar a lista;
	// op = 0, 1 ou 2, onde 0 - significa &nbsp&nbsp, 1 - significa <br>, 2 - significa <br><br>
	global ${$var};
	if ($habil <> 'N') $habil = '' ; else $habil='  disabled="disabled" ';
	$esp = '  ';
	if ($op == 1) $esp = '<br>' ;
	if ($op == 2) $esp = '<br><br>';
	$res=$this->carregar($sql);
	for ($i=0;$i<count($res);$i++) {
		if (${$var}== $res[$i]['codigo']) $sel = '  checked '; else $sel='';
		print "<input type='radio' value='".$res[$i]['codigo']."' name='".$var."'".$sel.$habil."/>".$res[$i]['descricao'].$esp;
	}
}

function record_set ($SQL)
{
	// este mtodo carrega em uma varivel o record set (Recurso) proveniente do sql
	$_SESSION['sql'] = $SQL;
	$res = @pg_query($this->link,$SQL);
	//			if( pg_errormessage( $this->link ) )
	//			{
	//				throw new Exception( pg_errormessage( $this->link ) );
	//			}
	return $res;
}

function carregaAgrupado($sql, $agrupador) {
	$rs = $this->record_set($sql);
	$retorno = array();
	while($linha = pg_fetch_assoc($rs)) {
		$retorno[$linha[$agrupador]][] = $linha;
	}
	return $retorno;
}

function conta_linhas ($recurso)
{
	// este mtodo conta as linhas de um recurso e subtrai 1
	$linhas = pg_num_rows($recurso);
	if( $linhas >= 0 )
	return $linhas-1;
	else
	return -1;
	//           if (! is_array(pg_fetch_all($recurso))) return -1;
	//           else return count(pg_fetch_all($recurso)) -1;
}

function pegaLinha($SQL, $linha=0) {
	//Retorna um registro de uma query, a partir da coluna especificada
	$_SESSION['sql'] = $SQL;
	if(($RS = pg_query($this->link,$SQL)) && (pg_num_rows($RS)>=1)) {
		return pg_fetch_assoc($RS, $linha);
	}
	else return false;
}

function pegaUm($SQL, $coluna=0) {
	//Retorna um registro de uma query, a partir da coluna especificada
	$_SESSION['sql'] = $SQL;
	if(($RS = pg_query($this->link,$SQL)) && (pg_num_rows($RS)>=1)) {
		return pg_fetch_result($RS, 0, $coluna);
	}
	else return false;
}

/**
 * Retorna o ano do exercício ativo.
 */
function pega_ano_atual()
{
	static $ano = false;
	if ( !$ano )
	{
		$esquema = $_SESSION['sisdiretorio'];
		$tabela = "programacaoexercicio";
		$sql =
			" select count(*) " .
			" from pg_class pc " .
			" inner join pg_namespace ns on " .
			" ns.oid = pc.relnamespace and " .
			" ns.nspname='" . $esquema . "' " .
			" where " .
			" relname = '" . $tabela . "'";
		$existe = (boolean) $this->pegaUm( $sql );
		if ( $existe )
		{
			$sql =
				" select prsano " .
				" from " . $esquema . "." . $tabela .
				" where prsexerccorrente = 't' " .
				" order by prsano desc " .
				" limit 1 ";
			$ano = (integer) $this->pegaUm( $sql );
		}
		if ( !$ano )
		{
			$ano = $this->pegaUm(
				"select ano from public.anos where anosnatual = 't' order by ano desc limit 1"
				);
		}
	}
	return $ano;
}

/**
 * Captura o nome das colunas de uma tabela.
 *
 * @param string $table
 * @param string $schema
 * @return string[]
 */
function pegarColunas( $table, $schema = 'public' )
{
	$table = str_replace( "'", "\\'", $table );
	$schema = str_replace( "'", "\\'", $schema );
	$sql = "select column_name from information_schema.columns where table_schema = '" . $schema . "' and table_name = '" . $table . "'";
	$linhas = $this->carregar( $sql );
	if ( !$linhas )
	{
		return array();
	}
	$colunas = array();
	foreach ( $linhas as $linha )
	{
		array_push( $colunas, $linha['column_name'] );
	}
	return $colunas;
}

/**
 * Captura as tabelas de um schema, caso o schema não seja indicado
 * todas as tabelas são retornadas.
 *
 * @param string $schema
 * @return string[]
 */
function pegarTabelas( $schema = null )
{
	$where = '';
	if ( $schema != null )
	{
		$schema = str_replace( "'", "\\'", $schema );
		$where = " where table_schema = '" . $schema. "' ";
	}
	$sql = 'select distinct table_name from information_schema.columns ' . $where;
	$linhas = $this->carregar( $sql );
	if ( !$linhas )
	{
		return array();
	}
	$tabelas = array();
	foreach ( $linhas as $linha )
	{
		array_push( $tabelas, $linha['table_name'] );
	}
	return $tabelas;
}

function carrega_registro ( $recurso, $posicao = 0 )
{
	// este metodo carrega em um array os campos de um registro
	$registro = array();
	if ( is_resource( $recurso ) && pg_num_rows( $recurso ) > 0 )
	{
		$registro = pg_fetch_array( $recurso, $posicao );
	}
	return $registro;
}

function carrega_tudo ($recurso)
{
	// este mtodo carrega em um array os campos de um registro
	return pg_fetch_all($recurso);
}

/*  function sucesso( $modulo, $parametros='')
 {
 if(! $modulo) {
 $saida = "history.back();";
 }
 else {
 $saida = $_SESSION['sisdiretorio'].'/'.$_SESSION['sisdiretorio'] . ".php?modulo=" . $modulo . "&acao=" . $_REQUEST['acao'] . $parametros;
 }

 header("Location: ../sucesso.php?saida=".urlencode($saida));
 exit();
 }
 */
function sucesso($modulo, $parametros='')
{
	?>
<html>
<head>
<script>
		alert('Operação realizada com sucesso');
              </script>
<script>
              <?if(!$modulo) {?>
              history.back();
              <?}else{
              	$saida = $_SESSION['sisarquivo'] . ".php?modulo=" . $modulo . "&acao=" . $_REQUEST['acao'] . $parametros;
              	?>
              	location.href="<?=$saida?>";
              	<?}?>
            </script>
</head>
<body>
 
</body>
</html>
              	<?
              	exit();
}

function insucesso( $mensagem='', $parametros='', $modulo='' )
{
	$modulo = $modulo ? $modulo : "inicio";
	$saida = $_SESSION['sisdiretorio'] . '/' . $_SESSION['sisdiretorio'] . ".php?modulo=" . $modulo . "&acao=C" . $parametros;
	$url = "../insucesso.php?saida=" . urlencode( $saida )."&mensagem=" . $mensagem;
	?>
<html>
<head>
<script type="text/javascript">
           				location.href = "<?= $url ?>";
           				</script>
</head>
<body>
 
</body>
</html>
	<?
	exit();
}

function rollback() {
	pg_query($this->link, 'rollback; ');
	unset($_SESSION['transacao']);
}

function commit() {
	if ($_SESSION['usucpf']==$_SESSION['usucpforigem'] or $_SESSION['superuser'] ){
		pg_query($this->link, 'commit; ');
		unset($_SESSION['transacao']);
		return true;
	} else {
		pg_query($this->link, 'rollback; ');
		unset($_SESSION['transacao']);
		return false;
	}
}

function close() {
	if ($_SESSION['transacao'])
	{
		pg_query($this->link, 'rollback; ');
		unset($_SESSION['transacao']);
	}
	if (is_resource($this->link)) {@pg_close( $this->link );}
}
/* Store login informations */
Function StoreLogin($username,$password) {
	/* Generate a mask and store the result */
	$data = $username . chr(1) . $password;
	$key = $this->randomkey(32);
	/* Store encrypted username and password */
	$_SESSION[serverauth] = criptografia::rc4($key,$data,"en");
	/* Store the mask */
	SetCookie( "cookieauth",base64_encode($key),0,"/");
	return 1;
}
Function GetLogin(&$username,&$password) {
	if( strlen( $_SESSION[serverauth] ) > 0 and  strlen( $_COOKIE[cookieauth] ) > 0 ) {
		$key=base64_decode(stripslashes($_COOKIE[cookieauth]));
		if( strlen( $key ) > 0 ) {
			$edata=stripslashes($_SESSION[serverauth]);
			$data=criptografia::rc4($key,$edata,"de");
			if( strlen( $data ) > 0 ) {
				$chr1pos = strpos( $data, chr( 1 ) );
				if( $chr1pos > 0 ) {
					$username = substr( $data, 0, $chr1pos );
					$password = substr( $data, $chr1pos + 1 );
				}
			}
		}
		return 1;
	} else
	return 0;
}
/* generate a random string (length $len) */
Function randomstr( $len ) {
	$c = "";
	for( $i = 0; $i < $len; $i++ )
	$c .= chr( rand( 0, 255 ) );
	return $c;
}
/* A random key */
Function randomkey( $len ) {
	srand( ( double )microtime() * 1000000 );
	return $this->randomstr( $len );
}
/* Retrieve login informations */

function testa_coordenador($acao,$t)
{
	// verifica se  coordenador de ao ou se  responsvel por subao
	if ($t == 'A')
	$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=1 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."' and rpustatus <> 'I' ";
	if ($t == 'S')
	$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=9 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."' and rpustatus <> 'I' ";

	$registro=$this->recuperar($sql);
	if (is_array($registro)) return true;
	else return false;
}

function testa_emenda()
{
	$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod=21 and pu.usucpf ='".$_SESSION['usucpf']."'";
	if ($this->pegaUm($sql)) return true;
	else return  false;
}

function testa_altagestao()
{
	$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod=11 and pu.usucpf ='".$_SESSION['usucpf']."'";
	if ($this->pegaUm($sql)) return true;
	else return  false;
}
function testa_altagestaopje($cod=0)
{
	// verifica se é alta-gestão ou gerente do projeto
	$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod in (58,12) and pu.usucpf ='".$_SESSION['usucpf']."' inner join monitora.usuarioresponsabilidade ur on ur.usucpf=pu.usucpf and ur.pjeid=".$_SESSION['pjeid'];
	if ($cod)
	{
		// então está testando um específico projeto
		$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod in (58,12) and pu.usucpf ='".$_SESSION['usucpf']."' inner join monitora.usuarioresponsabilidade ur on ur.usucpf=pu.usucpf and ur.pjeid=$cod";

	}
	if ($this->pegaUm($sql)) return true;
	else return  false;
}

function testa_coordenador_plan($cod)
{
	// verifica se  coordenador de planejamento ou super-usuario
	if ($this->testa_superuser())  return true;
	else {
		$sql= 'select usucpf from '.$_SESSION['sisdiretorio'].'.usuarioresponsabilidade where pflcod in (24,23) and acaid='.$cod." and usucpf = '".$_SESSION['usucpf']."'";

		$registro=$this->recuperar($sql);
		if (is_array($registro)) return true;
		else return false;
	}
}

function testa_gerente($prg,$t)
{
	// verifica se  gerente de programa
	$sql= 'select usucpf from '.$_SESSION['sisdiretorio'].'.usuarioresponsabilidade where pflcod=2 and prgid='.$prg." and usucpf = '".$_SESSION['usucpf']."'";
	$registro=$this->recuperar($sql);
	if (is_array($registro)) return true;
	else return false;
}


function testa_responsavel_projespec($cod=0)
{
	// verifica se  é responsavel por projeto especial ou super usuário
	if ($cod)
	{
		//
		$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod in (12,47,56) and pjeid='.$cod." and usucpf = '".$_SESSION['usucpf']."'";
	}
	else
	$sql= "select usucpf from seguranca.perfilusuario where pflcod in (62) and usucpf = '".$_SESSION['usucpf']."'";
	$resp = $this->pegaUm($sql);

	if ($resp) return true;
	else return false;
}


function testa_documentador_projespec($cod=0)
{
	// verifica se  é responsavel por documentar o projeto especial ou super usuário
	$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod in (75,56) and pjeid='.$cod." and usucpf = '".$_SESSION['usucpf']."'";
	$resp = $this->pegaUm($sql);
	if ($resp) return true;
	else return false;
}

function testa_gerente_exe($acao,$t)
{
	// verifica se  gerente-executivo de ao ou de programa
	if ($t == 'A')
	$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=3 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
	else
	$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=3 and prgid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
	$registro=$this->recuperar($sql);
	if (is_array($registro)) return true; else return false;
}

function testa_digitador($acao,$t)
{
	// verifica se  digitador de ao, de programa ou de subao ou projeto especial
	if ($t == 'A')
	$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=8 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
	if ($t == 'P')
	$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=8 and prgid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
	if ($t == 'S')
	$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=8 and saoid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
	if ($t == 'E')
	$sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=51 and pjeid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";

	$registro=$this->recuperar($sql);
	if (is_array($registro)) return true; else return false;
}

function testa_proprietario($cod)
{
	// verifica se  digitador é o proprietário do registro
	$sql= "select ptosntemdono as temdono,usucpf   from monitora.planotrabalho where ptoid=$cod ";
	$res=$this->pegalinha($sql);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	if ($temdono == 'f') return true;
	else if ($temdono == 't' and $usucpf==$_SESSION['usucpf']) return true;
	else return false;
}


function testa_superuser()
{ // testa se  super usurio
if ( !$_SESSION['usucpf'] || !$_SESSION['sisid'] ) {
	return false;
}
$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflsuperuser='t' and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
$registro=$this->recuperar($sql);
if (is_array($registro)) return true;
else return false;
}

function testa_coorduma()
{ // testa se  é coordenador da UMA
$sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod=4 and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
$registro=$this->recuperar($sql);
if (is_array($registro)) return true;
else return false;
}
function testa_uma()
{
	// verifica se  perfil UMA ou coordenador da UMA ou superusurio (pode emular outro usuário)
	//to do:esta função deverá se substituida passará a fazer a verificação por uma flag na tabela perfil (suporte:bool)
	if ($_SESSION['sisid']==1)
	$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (4,6,18) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
	else if ($_SESSION['sisid']==2)
	$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (50,52) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
	else if ($_SESSION['sisid']==5) // elaboração e revisão
	$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (62,34,36) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
	else if ($_SESSION['sisid']==12) // elaboração e revisão
	$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (111) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
	else if ($_SESSION['sisid']==10) // elaboração e revisão
	$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (85) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
	else if ($_SESSION['sisid']==13) // elaboração e revisão
	$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (125) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
	$registro=$this->recuperar($sql);
	if (is_array($registro)) return true;
	else return false;
}

function testa_cgo()
{
	// verifica se  perfil CGO  ou equipe cgo
	if ($_SESSION['sisid']==1)
	$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (4,6,18) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
	else if ($_SESSION['sisid']==2)
	$sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (50,52) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
	$registro=$this->recuperar($sql);
	if (is_array($registro)) return true;
	else return false;
}

function testa_responsabilidade($id,$pflcod, $campo)
{
	// verifica se  gerente de programa
	$sql= "select usucpf from usuarioresponsabilidade where pflcod=$pflcod and $campo = '$id' and usucpf = '".$_SESSION['usucpf']."';";
	$registro=$this->recuperar($sql);
	if (is_array($registro)) return true;
	else return false;
}

function mostra_resp( $chave_valor, $chave_nome,$ano=true,$schema='monitora' )
{
	// carrega os registros
	if ($ano){
	$sql = sprintf(
		"select distinct p.pflnivel, p.pfldsc, usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum, uni.unidsc
				from seguranca.perfil p 
				inner join $schema.usuarioresponsabilidade ur on ur.pflcod = p.pflcod
				inner join seguranca.usuario usu on usu.usucpf = ur.usucpf 
				inner join unidade uni on uni.unicod = usu.unicod
				where p.pflstatus = 'A' and ur.%s = '%s' and ur.rpustatus = 'A' and p.pflresponsabilidade != 'H' and ur.prsano = '%s'
				order by p.pflnivel",
	$chave_nome,
	$chave_valor,
	$_SESSION['exercicio']
	);
	

	}else
	$sql = sprintf(
		"select distinct p.pflnivel, p.pfldsc, usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum, uni.unidsc
				from seguranca.perfil p 
				inner join $schema.usuarioresponsabilidade ur on ur.pflcod = p.pflcod
				inner join seguranca.usuario usu on usu.usucpf = ur.usucpf 
				inner join unidade uni on uni.unicod = usu.unicod
				where p.pflstatus = 'A' and ur.%s = '%s' and ur.rpustatus = 'A' and p.pflresponsabilidade != 'H' 
				order by p.pflnivel",
	$chave_nome,
	$chave_valor
	);
	$responsaveis = $this->carregar( $sql );
	if ( !$responsaveis ) {
		return;
	}

	// exibe o primeiro registro
	echo "<script>
				function exibirEquipeApoio(){
					elemento = document.getElementById( 'responsaveis' );
					imagem = document.getElementById( 'botao_mais_menos' );
					if ( elemento.style.display == 'block' ) {
						elemento.style.display = 'none';
						imagem.src = '../imagens/mais.gif';
					} else {
						imagem.src = '../imagens/menos.gif';
						elemento.style.display = 'block';
					}
				}
			</script>";
	$responsavel = array_shift( $responsaveis );
	$htm = sprintf(
		"<tr><td width='250' align='right' class='SubTituloDireita'><a href='#' title='exibir equipe de apoio' onclick='exibirEquipeApoio();'><img id='botao_mais_menos' src='../imagens/mais.gif' border='0'/></a> %s:</td><td><img src='../imagens/email.gif' title='Enviar e-mail ao Gestor' border='0' onclick='envia_email(\"%s\");'> %s<br><font color=#888888>%s - Tel: (%s) %s</font></td></tr>",
	$responsavel['pfldsc'],
	$responsavel['usucpf'],
	$responsavel['usunome'],
	$responsavel['unidsc'],
	$responsavel['usufoneddd'],
	$responsavel['usufonenum']
	);
	echo $htm;

	// exibe os demais registros
	echo '<tr><td colspan="2" style="border: 0; padding: 0;"><div id="responsaveis" style="display: none"><table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%; border: 0">';
	foreach ( $responsaveis as $indice => $responsavel ) {
		// monta o html
		$htm = sprintf(
			"<tr><td width='250' align='right' class='SubTituloDireita' width='20%%'>%s:</td><td><img src='../imagens/email.gif' title='Enviar e-mail ao Gestor' border='0' onclick='envia_email(\"%s\");'> %s<br><font color=#888888>%s - Tel: (%s) %s</font></td></tr>",
		$responsavel['pfldsc'],
		$responsavel['usucpf'],
		$responsavel['usunome'],
		$responsavel['unidsc'],
		$responsavel['usufoneddd'],
		$responsavel['usufonenum']
		);
		echo $htm;
	}
	echo "</table></div></td></tr>";
}


function cabecalho_projeto($pjeid)
{
	$sql = "select p.*,u.ungabrev from monitora.projetoespecial p inner join unidadegestora u using (ungcod) where pjeid=".$pjeid;
	$RS = $this->record_set($sql);
	$nlinhas = $this->conta_linhas($RS);
	$nl=$nlinhas;
	//if ($nlinhas >= 0) {
	$res =  $this->carrega_registro($RS,0);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	// a linha abaixo transforma em variÃ¡veis todos os campos do array

	?>
<tr>
	<td align='right' class="subtitulodireita" width="20%">Denominação:</td>
	<td><?=$ungabrev.'-'.$pjecod.'  '.$pjedsc?></td>
</tr>
	<?
	@$this -> mostra_resp($pjeid, 'pjeid',false);
}


function cabecalho_acao($acaid)
{

	$sql = "select * from acao where acaid=".$acaid;
	$RS = $this->record_set($sql);
	$nlinhas = $this->conta_linhas($RS);
	$nl=$nlinhas;
	//if ($nlinhas >= 0) {
	$res =  $this->carrega_registro($RS,0);
	// a linha abaixo transforma em variÃ¡veis todos os campos do array
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	$unidsc = $this->pegaUm( "select unidsc from public.unidade where unicod = '" . $unicod . "'" );
	?>
<tr>
	<td align='right' class="subtitulodireita" width="20%">Ação:</td>
	<td><?=$prgcod.'.'.$acacod.'.'.$unicod.'.'.$loccod.' - '.$acadsc?></td>
</tr>
</tr>
<td align='right' class="subtitulodireita">Unidade:</td>
<td><?=$unicod.' - '.$unidsc?></td>
</tr>
	<?
	@$this -> mostra_resp($acaid, 'acaid');
}

function cabecalho_programa($prgid)
{

	$sql = "select * from monitora.programa where prgid=".$prgid;
	$RS = $this->record_set($sql);
	$nlinhas = $this->conta_linhas($RS);
	$nl=$nlinhas;
	//if ($nlinhas >= 0) {
	$res =  $this->carrega_registro($RS,0);
	// a linha abaixo transforma em variÃ¡veis todos os campos do array
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	$orgdsc = $this->pegaUm( "select orgdsc from public.orgao where orgcod = '" . $orgcod . "' limit 1" );
	?>
<tr>
	<td align='right' class="subtitulodireita" width="20%">Programa:</td>
	<td><?=$prgcod . ' - ' . $prgdsc?></td>
</tr>
</tr>
<td align='right' class="subtitulodireita">Órgão:</td>
<td><?=$orgcod.' - '.$orgdsc?></td>
</tr>
	<?
	@$this -> mostra_resp($prgid, 'prgid');
}


function relatsubacao($res,$acaoid)
{

	unset($inicio,$fim,$soma);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	$nivel = $this->busca_pai($ptoid,0);
	if (! in_array($ptoid,$_SESSION['ptoid']))

	{
		if ($ptotipo=='S' and $nivel==0)
		{
			include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosfisevo2.inc";
			$sql = "select p.acaid,p.ptoid, ptoid_pai,ptotipo,p.ptocod, case when p.ptotipo='S' then 'Subao' when p.ptotipo='E' then 'Etapa' else 'Fase' end as tipo,p.ptodsc, p.ptoprevistoexercicio as previsto, p.ptosnpercent, p.ptosnsoma, u.unmdsc, case when sum(e.exprealizado) is null then 0 else sum(e.exprealizado) end as totalrealizado,to_char(p.ptodata_ini,'DD/MM/YYYY') as inicio, to_char(p.ptodata_fim,'DD/MM/YYYY') as fim from planotrabalho p inner join unidademedida u on p.unmcod=u.unmcod left join execucaopto e on p.ptoid=e.ptoid where p.ptostatus='A' and p.ptoid_pai=".$ptoid."  and p.acaid=".$acaoid."  group by p.acaid,p.ptoid_pai,p.ptoid,p.ptotipo, p.ptocod, p.ptodsc, p.ptoprevistoexercicio, p.ptosnpercent, p.ptosnsoma,p.ptoordem, u.unmdsc,p.ptodata_ini,p.ptodata_fim order by p.ptoordem,p.ptotipo desc,p.ptoid_pai, p.ptocod";

			$RSp = $this->record_set($sql);
			$nlinhasp = $this->conta_linhas($RSp);
			for ($im=0; $im<=$nlinhasp;$im++)
			{
				$res = $this->carrega_registro($RSp,$im);
				$this->relatsubacao($res,$acaoid);
			}
		}
		else if ($nivel==1)
		include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosfisevo2.inc";
		else if ($nivel==2) {
			if (in_array($ptoid_pai,$_SESSION['ptoid']))
			include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosfisevo2.inc";
		}
	}
}


// Returns true if $string is valid UTF-8 and false otherwise.
function is_utf8($string) {

	// From http://w3.org/International/questions/qa-forms-utf-8.html
	return preg_match('%^(?:
         [\x09\x0A\x0D\x20-\x7E]            # ASCII
       | [\xC2-\xDF][\x80-\xBF]            # non-overlong 2-byte
       |  \xE0[\xA0-\xBF][\x80-\xBF]        # excluding overlongs
       | [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}  # straight 3-byte
       |  \xED[\x80-\x9F][\x80-\xBF]        # excluding surrogates
       |  \xF0[\x90-\xBF][\x80-\xBF]{2}    # planes 1-3
       | [\xF1-\xF3][\x80-\xBF]{3}          # planes 4-15
       |  \xF4[\x80-\x8F][\x80-\xBF]{2}    # plane 16
   )*$%xs', $string);

} // function is_utf8

function converttoxml( &$dbconnection, $strsql, $filename, $arquivo )
{
	ini_set( 'memory_limit', '128M' );
	$rs = pg_query($strsql);
	$nlinhas = !is_array( pg_fetch_all( $rs ) ) ? -1 : count( pg_fetch_all( $rs ) ) - 1;
	$dir = IS_PRODUCAO ? '/var/www/html/simec/arquivos/SIGPLAN/exportacao/' : '/var/www/html/simec/arquivos/SIGPLAN/exportacao/';
	$filename = basename( $filename, '.xml' ) . date( '-d-m-y' ) . '.xml';
	$path = $dir . $filename;
	$xml_file = fopen( $path, 'w' );
	if ( !$xml_file )
	{
		return 0;
	}
	$xml = '<?xml version="1.0" encoding="utf-8"?'.">\n";
	$xml .= '<ArrayOf' . $arquivo . ' xmlns="http://www.sigplan.gov.br/xml/">';
	//$xml.='<ArrayOf'.$arquivo.' xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://www.sigplan.gov.br/xml/">';
	$sucesso = fwrite( $xml_file, $xml );
	for ( $j = 0; $j <= $nlinhas; $j++ )
	{
		$xml = '';
		$res = pg_fetch_array( $rs, $j );
		if( is_array( $res ) )
		{
			$xml .= "\n  <" . $arquivo. ">\n";
			foreach( $res as $k => $v )
			{
				${$k} = $v;
				if ( $v == 't' )
				{
					$v = '1';
				}
				elseif ( $v == 'f' )
				{
					$v = '0';
				}
				if ( !is_int( $k ) && $v != '' )
				{
					$v = utf8_encode( simec_htmlspecialchars( $v ) );
					$v = str_replace( "'", "&apos;", $v );
					$xml .= "    <" . $k . ">" . $v . "</" . $k . ">\n";
				}
			}
			$xml .= "  </" . $arquivo . ">";
			$sucesso = fwrite( $xml_file, $xml );
		}
	}
	$xml = "\n</ArrayOf" . $arquivo . ">";
	$sucesso = fwrite( $xml_file, $xml );
	fclose( $xml_file );
	return $sucesso > 0 ? 1 : 0;
}




function chama_etapafase($cod)
{
	unset($referencias);
	$referencias = array();
	if ($coordaca or $digit){
		$sql = "select refcod from referencia where cast(current_date as date)>=refdata_inicio and cast(current_date as date)<=refdata_limite_avaliacao_aca";
		$RS = $db->record_set($sql);
		$nlinhas = $db->conta_linhas($RS);
		if ($nlinhas >= 0) {
			for ($i=0; $i<=$nlinhas;$i++){
				$res = $db->carrega_registro($RS,$i);
				foreach($res as $k=>$v) ${$k}=$v;
				array_push($referencias, $v);
			}
		}
	}
	$sql = "select p.ptoid, p.ptocod, p.ptodsc, p.ptoprevistoexercicio as previsto, p.ptosnpercent, p.ptosnsoma, u.unmdsc, case when sum(e.exprealizado) is null then 0 else sum(e.exprealizado) end as totalrealizado from planotrabalho p inner join unidademedida u on p.unmcod=u.unmcod left join execucaopto e on p.ptoid=e.ptoid where p.ptoid = $cod and p.acaid=".$_SESSION['acaid']."  group by p.ptoid, p.ptocod, p.ptodsc, p.ptoprevistoexercicio, p.ptosnpercent, p.ptosnsoma, u.unmdsc";
	$RS = $this->record_set($sql);
	$nlinhas = $this->conta_linhas($RS);
	if ($nlinhas >= 0)
	{
		$res = $this->carrega_registro($RS,0);
		foreach($res as $k=>$v) ${$k}=$v;
		$porcentorealizado = $totalrealizado*100/$previsto;
		if ($porcentorealizado > 100) $mostraporcentorealizado = 100; else $mostraporcentorealizado = $porcentorealizado;
		$porcentoexecutado = 100 - $mostraporcentorealizado;?>
<THEAD bgcolor="#f5f5f5">
	<tr style="background-color: #ececec;">
		<TD colspan="12" align="left" style="color: #000099;">Cd.: <strong><?=$ptocod?>
		- <?=$ptodsc?></strong></TD>
		<TD colspan="2" align="right">Unid. Medida:<strong><?=$unmdsc?></strong><br>
		0% <label
			style="border: 1px solid #000000; font-size: 8px; border-top: 1px solid #c0c0c0; border-right: 1px solid #c0c0c0; border-left: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; background-color: #ffffff;"
			title="Realizado <?=number_format($porcentorealizado, 0, '.', '')?>%"><span
			style="background-color: #33cc00; font-size: 8px;"><?for ($k=0; $k<$mostraporcentorealizado/2;$k++) print " ";?></span><?for ($k=0; $k<$porcentoexecutado/2;$k++) print " ";?></label>
			<?
			if ($porcentorealizado < 100) print number_format($porcentorealizado, 0, '.', ''); else print '100'; ?>%</TD>
	</tr>
</THEAD>
<TBODY>
	<TR style="color: #808080;" bgcolor="#f6f6f6">
	<?
	$sql = "select refmes_ref, refano_ref from referencia where refdata_limite_avaliacao_aca is not null and refsnmonitoramento='t'   and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
	$RS2 = $this->record_set($sql);
	$nlinhas2 = $this->conta_linhas($RS2);
	if ($nlinhas2 >= 0)
	{
		for ($j=0; $j<=$nlinhas2;$j++)
		{
			$res2 = $this->carrega_registro($RS2,$j);
			foreach($res2 as $k=>$v) ${$k}=$v;
			?>
		<TD align="right"><?=$refmes_ref.'/'.$refano_ref?></TD>
		<?  }
}
$colspan = 12 - $j;
if ($colspan>0) print "<TD colspan='".$colspan."'></TD>";
?>
		<TD align="right">Realizado</TD>
		<TD align="right">Previsto</TD>
	</TR>
	<TR style="height: 30px;">
	<?
	$sql = "select r.refcod, case when e.exprealizado is null then -1 else e.exprealizado end as exprealizado,  e.expobs, e.tpscod, t.tpscod, t.tpsdsc from referencia r left join execucaopto e on r.refcod=e.refcod and e.ptoid=".$ptoid." left join tiposituacao t on e.tpscod=t.tpscod where r.refdata_limite_avaliacao_aca is not null and r.refsnmonitoramento='t' and r.refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
	$RS2 = $this->record_set($sql);
	$nlinhas2 = $this->conta_linhas($RS2);
	$totalrealizado = 0;
	if ($nlinhas2 >= 0)
	{
		for ($j=0; $j<=$nlinhas2;$j++)
		{
			$res2 = $this->carrega_registro($RS2,$j);
			foreach($res2 as $k=>$v) ${$k}=$v;
			if ($exprealizado == -1) $v_exprealizado=$exprealizado+1; else $v_exprealizado=$exprealizado;
			if (in($refcod,$referencias) and ($coordaca or $digit)) $txtexprealizado = '<input type="Text" size="5" value="'.$v_exprealizado.'" class="CampoEstilo" style="text-align : right; color:#808080;"  readonly="readonly" onclick="edita(\''.$refcod.'\',\''.$ptoid.'\');" onmouseover="MouseOver(this);" onmouseout="MouseOut(this);"/>'; else $txtexprealizado = $v_exprealizado;?>
		<TD align="right" style="color: #3366ff;"><?=$txtexprealizado?></TD>
		<?	if ($ptosnsoma=='r') { if($exprealizado>-1) $totalrealizado = $exprealizado;}
		else
		{
			if ($exprealizado == -1) $exprealizado=$exprealizado+1;
			$totalrealizado = $totalrealizado + $exprealizado;
		}
}
}
$colspan = 12 - $j;
if ($colspan>0) print "<TD colspan='".$colspan."'></TD>";
?>
		<TD align="right" style="color: #cc0000;"><?=number_format($totalrealizado, 0, '.', '');?></TD>
		<TD align="right" style="color: #0000CC;"><?=$previsto?></TD>
	</TR>
</TBODY>
<tr>
	<td colspan="14"
		style="height: 1px; background-color: #e5e5e5; padding: 0px"></td>
</tr>
<tr>
	<td colspan="14"
		style="height: 2px; background-color: #000000; padding: 0px"></td>
</tr>
<?
}
}


function chama_subacao($cod)
{
	print ' uma subao'.$cod;
}


/**
 * Verifica se o usuário possui algum perfil que permite manipular
 * todas as ações. Os perfis que permitem essa manipulação são os
 * que não possuem registros em elabrev.tprperfil.
 *
 * @return boolean
 */
function usuarioPossuiPermissaoTodasUnidades( $esquema = 'elabrev' )
{
	$cpf = $_SESSION['usucpf'];
	$sisid = $_SESSION['sisid'];
	$sql =
			"select count(*)
			from seguranca.perfilusuario
				inner join seguranca.perfil using ( pflcod )
				left join " . $esquema . ".tprperfil using ( pflcod )
			where
				usucpf = '" . $cpf . "' and
				tprcod is null and
				sisid = " . $sisid . " ";
	return (boolean) $this->pegaUm( $sql );
}

/**
 * Verifica se o usuário possui algum perfil que permite manipular
 * alguma unidade. Os perfis que permitem essa manipulação são os
 * que possuem registros em elabrev.tprperfil com o tprcod = 9.
 *
 * @return boolean
 */
function usuarioPossuiPermissaoAlgumaUnidade( $esquema = 'elabrev' )
{
	$cpf = $_SESSION['usucpf'];
	$sql = "select count(*) from seguranca.perfilusuario left join " . $esquema . ".tprperfil using ( pflcod ) where usucpf = '" . $cpf . "'";
	return (boolean) $this->pegaUm( $sql );
}

/**
 * Verifica se usuário pode manipular uma determinada unidade.
 *
 * @param string $unicod
 * @return boolean
 */
function usuarioPossuiPermissaoUnidade( $unicod, $esquema = 'elabrev' )
{
	if ( $this->usuarioPossuiPermissaoTodasUnidades( $esquema ) )
	{
		return true;
	}
	if ( !$this->usuarioPossuiPermissaoAlgumaUnidade( $esquema ) )
	{
		return false;
	}
	$unicod = str_replace( "'", "\\'", $unicod );
	$cpf = $_SESSION['usucpf'];
	$sql = "select count(*) from " . $esquema . ".usuarioresponsabilidade where usucpf = '" . $cpf . "' and rpustatus = 'A' and unicod = '" . $unicod . "'";
	return (boolean) $this->pegaUm( $sql );
}

/**
 * Monta join a ser utilizado por algum outro query com as unidades do
 * usuário. não é possível adivinhar qual o nome da tabela que conterá
 * o unicod para fazer a restrição, portanto A RESTRICAO DO UNICOD DEVE
 * SER ADICIONADO AO FINAL DO RETORNO.
 *
 * Exemplo:
 * $join = $db->usuarioJoinUnidadesPermitidas();
 * $join .= " and minhatabela.unicod = unijoin.unicod ";
 *
 * O nome das tabelas são unijoin (unidade) e usujoin
 * (usuarioresponsabilidade). Este segundo só aparece caso o usuário não
 * possui um perfil que permita trabalhar com todas as unidades
 *
 * @return string
 */
function usuarioJoinUnidadesPermitidas( $esquema = 'elabrev' )
{
	$join = "";
	$podeTodas = $this->usuarioPossuiPermissaoTodasUnidades( $esquema );
	if ( !$podeTodas )
	{
		$join .=
				" inner join " . $esquema . ".usuarioresponsabilidade usujoin on " .
					" usujoin.usucpf = '" . $_SESSION['usucpf'] . "' and " .
					" usujoin.rpustatus = 'A' and " .
					" usujoin.pflcod in ( " .
						" select pflcod " .
						" from seguranca.perfilusuario " .
						" where usucpf = '" . $_SESSION['usucpf'] . "'" .
					" ) ";
	}
	$join .=
			" inner join unidade unijoin on " .
				" unijoin.unicod != '26100' and " .
				" unijoin.unicod != '". CODIGO_ORGAO_SISTEMA. "' and " .
				" unijoin.orgcod = '". CODIGO_ORGAO_SISTEMA. "' ";
	if ( !$podeTodas )
	{
		$join .= " and unijoin.unicod = usujoin.unicod ";
	}
	return $join;
}

/**
 * Captura os códigos da unidades que o usuário pode manipular.
 *
 * @return string[]
 */
function usuarioUnidadesPermitidas( $esquema = 'elabrev' )
{
	if ( $this->usuarioPossuiPermissaoTodasUnidades( $esquema ) )
	{
		$sql = "
				select unicod
				from unidade
				where
					unistatus = 'A' and
					(
						( orgcod = '". CODIGO_ORGAO_SISTEMA. "' and unicod != '26100' )
						or
						( unicod = '74902' )
					)
			";
	}
	else
	{
		$sql =
			" select " .
			" distinct unicod " .
			" from " . $esquema . ".usuarioresponsabilidade " .
			" inner join unidade using ( unicod ) " .
			" inner join seguranca.perfilusuario using ( pflcod, usucpf ) " .
			" where " .
			" usucpf = '" . $_SESSION['usucpf'] . "' and " .
			" rpustatus = 'A' and " .
			" unistatus = 'A' and " .
			" orgcod = '". CODIGO_ORGAO_SISTEMA. "' and " .
			" unicod != '26100' ";
	}
	if ( !$this->usuarioPossuiPermissaoAlgumaUnidade( $esquema ) )
	{
		return array();
	}
	$dados =  $this->carregar( $sql );
	$retorno = array();
	if ( $dados )
	{
		foreach ( $dados as $dado )
		{
			array_push( $retorno, $dado['unicod'] );
		}
	}
	return $retorno;
}

public function alterar_status_usuario( $cpf, $status, $justificativa, $sisid = null ){
	// $sql_status é a query que altera o status em algum sistema/geral
	// $sql_cascata é a query que altear o status em alguma sistema/geral devido a alteração inicialmente definida
	// $sql_historico é a query que gera histórico para a alteração que está sendo efetuada
	// $sql_historico_cascata é a query que gera histórico para as alterações que estão sendo efetuadas em cascata
	$usucpfadm = $cpf != $_SESSION['usucpf'] ? "'" . $_SESSION['usucpf'] . "'" : 'null';
	$sql_cascata = '';
	$sql_historico_cascata = array();
	if ( $sisid ) {
		$sql_status = sprintf(
			"UPDATE seguranca.usuario_sistema SET suscod = '%s' WHERE sisid = %d AND usucpf = '%s'",
		$status,
		$sisid,
		$cpf
		);
		$sql_historico = sprintf(
			"INSERT INTO seguranca.historicousuario ( htudsc, usucpf, sisid, suscod, usucpfadm ) VALUES ( '%s', '%s', %d, '%1s', %s )",
		$justificativa,
		$cpf,
		$sisid,
		$status,
		$usucpfadm
		);
		if ( $status == 'A' ) // verifica se está alterando para ativo
		{
			$sql_status_geral = sprintf(
				"SELECT suscod FROM seguranca.usuario WHERE usucpf = '%s'",
			$cpf
			);
			$status_geral = $this->pegaUm( $sql_status_geral );
			if ( $status_geral != 'A' ) // verifica se status geral não é ativo
			{
				// altera em cascata no geral
				$sql_cascata = sprintf(
					"UPDATE seguranca.usuario SET suscod = '%s' WHERE usucpf = '%s'",
				$status,
				$cpf
				);
				$sql_historico_cascata = array( sprintf(
					"INSERT INTO seguranca.historicousuario ( htudsc, usucpf, suscod, usucpfadm ) VALUES ( '%s', '%s', '%1s', %s )",
				$justificativa,
				$cpf,
				$status,
				$usucpfadm
				) );
			}
		}
	} else {
		$sql_status = sprintf(
			"UPDATE seguranca.usuario SET suscod = '%s' WHERE usucpf = '%s'",
		$status,
		$cpf
		);
		$sql_historico = sprintf(
			"INSERT INTO seguranca.historicousuario ( htudsc, usucpf, suscod, usucpfadm ) VALUES ( '%s', '%s', '%1s', %s )",
		$justificativa,
		$cpf,
		$status,
		$usucpfadm
		);
		if ( $status != 'A' )
		{
			// altera em cascata para todos os sistemas
			$sql_cascata = sprintf(
				"UPDATE seguranca.usuario_sistema SET suscod = '%s' WHERE usucpf = '%s'",
			$status,
			$cpf
			);
			// captura os sistemas do usuário
			$sql_sistemas_usuario = sprintf(
				"SELECT DISTINCT sisid, suscod FROM seguranca.usuario_sistema WHERE usucpf = '%s'",
			$cpf
			);
			$sistemas = $this->carregar( $sql_sistemas_usuario );
			foreach ( $sistemas as $sistema )
			{
				if ( $sistema['suscod'] == $status ) // verifica se precisa gerar historico
				{
					continue;
				}
				array_push(
				$sql_historico_cascata,
				sprintf(
					"INSERT INTO seguranca.historicousuario ( htudsc, usucpf, sisid, suscod, usucpfadm ) VALUES ( '%s', '%s', %d, '%1s', %s )",
				$justificativa,
				$cpf,
				$sistema['sisid'],
				$status,
				$usucpfadm
				)
				);
			}
		}
	}
	$this->executar( $sql_status );
	if ( $sql_cascata != '' )
	{
		$this->executar( $sql_cascata );
	}
	if ( count( $sql_historico_cascata ) > 0 )
	{
		foreach ( $sql_historico_cascata as $sql )
		{
			$this->executar( $sql );
		}
	}

	$this->executar( $sql_historico );

}

public function gerar_senha(){
	$dicionario = array(
		'a','b','c','d','e','f','g','h','i','j','l','m','n','o','p','q','r','s','t','u','v','x','z'
		);
		$j = rand( 4, 8 );
		$senha = '';
		for( $i=0; $i < $j ;$i++ ) {
			$senha .= $dicionario[array_rand( $dicionario )];
		}
		return $senha;
}

}

class criptografia {
	function rc4($pwd, $data, $case) {
		if ($case == 'de') {
			$data = urldecode($data);
		}
		$key[] = "";
		$box[] = "";
		$temp_swap = "";
		$pwd_length = 0;
		$pwd_length = strlen($pwd);
		for ($i = 0; $i <= 255; $i++) {
			$key[$i] = ord(substr($pwd, ($i % $pwd_length), 1));
			$box[$i] = $i;
		}
		$x = 0;
		for ($i = 0; $i <= 255; $i++) {
			$x = ($x + $box[$i] + $key[$i]) % 256;
			$temp_swap = $box[$i];
			$box[$i] = $box[$x];
			$box[$x] = $temp_swap;
		}
		$temp = "";
		$k = "";
		$cipherby = "";
		$cipher = "";
		$a = 0;
		$j = 0;
		for ($i = 0; $i < strlen($data); $i++) {
			$a = ($a + 1) % 256;
			$j = ($j + $box[$a]) % 256;
			$temp = $box[$a];
			$box[$a] = $box[$j];
			$box[$j] = $temp;
			$k = $box[(($box[$a] + $box[$j]) % 256)];
			$cipherby = ord(substr($data, $i, 1)) ^ $k;
			$cipher .= chr($cipherby);
		}
		if ($case == 'de') {
			$cipher = urldecode(urlencode($cipher));
		} else {
			$cipher = urlencode($cipher);
		}
		return $cipher;
	}
	function hash_senha($senha) {
		return "{MD5}".base64_encode(pack("H*",md5($senha)));
	}
}

/**
 * Agrupadora de testes e iterador. Permite dizer se uma bateria de testes foi
 * realizada com sucesso, alm de permitir percorrer os testes realizados
 * @author Adonias Malosso <adonias@mesote.com.br>
 * @version 1.0
 */
class Testes implements Iterator {
	private $_testes;

	public function __construct() {
		$this->_testes = array();
	}

	public function addTeste(Teste $teste) {
		$this->_testes[] = $teste;
	}

	public function delTeste($pos) {
		if(isset($this->_testes[$pos]))
		unset($this->_testes[$pos]);
	}

	public function rewind() {
		reset($this->_testes);
	}

	public function current() {
		$var = current($this->_testes);
		return $var;
	}

	public function key() {
		$var = key($this->_testes);
		return $var;
	}

	public function next() {
		$var = next($this->_testes);
		return $var;
	}

	public function valid() {
		$var = $this->current() !== false;
		return $var;
	}

	public function resultado() {
		foreach ($this->_testes as $teste)
		if(!$teste->status)
		return false;

		return true;
	}
}


class  GeraExcel{

	// define parametros(init)
	function  GeraExcel(){
		global $nomeDoArquivoXls;
		$this->armazena_dados   = ""; // Armazena dados para imprimir(temporario)
		$this->nomeDoArquivoXls = $nomeDoArquivoXls; // Nome do arquivo excel
		$this->ExcelStart();
	}// fim constructor


	// Monta cabecario do arquivo(tipo xls)
	function ExcelStart(){

		//inicio do cabecario do arquivo
		$this->armazena_dados = pack( "vvvvvv", 0x809, 0x08, 0x00,0x10, 0x0, 0x0 );
	}

	// Fim do arquivo excel
	function FechaArquivo(){
		$this->armazena_dados .= pack( "vv", 0x0A, 0x00);
	}




	// monta conteudo
	//function MontaConteudoString( $excel_linha, $excel_coluna, $value){
	function MontaConteudoString( $excel_linha, $excel_coluna, $value){
		$tamanho = strlen($value);
		$this->armazena_dados .= pack( "v*", 0x0204, 8 + $tamanho, $excel_linha, $excel_coluna, 0x00, $tamanho );
		$this->armazena_dados .= $value;
	}//Fim, monta Col/Lin

	function MontaConteudoNumero($row, $col, $num)
	{
		$record    = 0x0203;
		$length    = 0x000E;
		$xf	= 0x0F;

		$header    = pack("vv",  $record, $length);
		$data      = pack("vvv", $row, $col, $xf);
		$xl_double = pack("d",   $num);
		$this->armazena_dados .= ($header.$data.$xl_double);
	}

	// Gera arquivo(xls)
	function GeraArquivo(){

		//Fecha arquivo(xls)
		$this->FechaArquivo();

		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=$this->nomeDoArquivoXls".".xls");
		header ( "Content-Disposition: attachment; filename=$this->nomeDoArquivoXls".".xls");
		header ( "Content-Description: MID Gera excel" );
		print  ( $this->armazena_dados);


	}// fecha funcao
	# Fim da classe que gera excel
}


/**
 * Abstrai o conceito de teste atravs de nome, informao, status e msg de erro.
 *
 * @author Adonias Malosso <adonias@mesotec.com.br>
 * @example $t = new Teste("Permisso de escrita no diretrio", "/var/log", is_writeable("/var/log");
 * @version 1.0
 */
class Teste {
	const IMGOK = "<img src='../imagens/valida1.gif' border='0' title='OK'/>";
	const IMGERRO = "<img src='../imagens/valida3.gif' border='0' title='Erro'/>";
	public $nome;
	public $info;
	public $status;
	public $msg;

	public function __construct($pnome, $pinfo, $pstatus, $pmsg="") {
		$this->nome = $pnome;
		$this->info = $pinfo;
		$this->status = $pstatus;
		$this->msg = $pmsg;
	}

	public function getImagem() {
		switch ($this->status) {
			case 0:
				return self::IMGERRO;
				break;
			case 1:
				return self::IMGOK;
				break;
		}
	}
}




include_once APPRAIZ . 'includes/workflow.php';

/*
if ( $_SESSION['usucpf'] != '' && $_SESSION['usucpf'] != '' )
{
header( "Location: ../manutencao.htm" );
die();
}
*/

// carrega as funções do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


/*
 Sistema Sistema Simec
 Setor responsável: SPO/MEC
 Desenvolvedor: Desenvolvedores Simec
 Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
 Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
 Módulo: classes_simec.inc
 Finalidade: reunião de todas as classes do sistema
 Data de criação: 24/06/2005
 */


?>



<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<?php



print '<br/>';

//monta_titulo( $titulo_modulo, '&nbsp;' );

$ptostatus = isset( $_REQUEST['ptostatus'] ) ? $_REQUEST['ptostatus'] : 'A';

$sql = "
		select
			d.dimid,
			d.dimcod || ' - ' || d.dimdsc as dimensao,
			area.ardcod || ' - ' || area.arddsc as area,
			area.ardid,			
			i.indcod || ' - ' || i.inddsc as indicador,
			i.indid,
			i.indcod,
			c.ctrpontuacao as pontuacao,
			p.ptojustificativa,
			p.ptodemandamunicipal,
			p.ptodemandaestadual,
			Case acao.acilocalizador
			when 'E' then 'Estadual'
			when 'M' then 'Municipal'
			end as Tipo,
			acao.aciid,
			acao.ptoid,
			acao.acidsc,
			acao.acirpns,
			acao.acicrg,
			to_char(acao.acidtinicial,'dd/mm/yyyy') as acidtinicial,
			to_char(acao.acidtfinal,'dd/mm/yyyy') as acidtfinal,
			acao.acirstd,
			acao.acilocalizador,
			subacao.sbaid,
			subacao.undid,
			subacao.frmid,
			subacao.sbadsc,
			subacao.sbastgmpl,
			--prg.prgdsc as sbaprm,
			sbaprm,
			subacao.sbapcr,
			coalesce(subacao.sba0ano, 0) as sba0ano,
			coalesce(subacao.sba1ano, 0) as sba1ano,
			coalesce(subacao.sba2ano, 0) as sba2ano,
			coalesce(subacao.sba3ano, 0) as sba3ano,
			coalesce(subacao.sba4ano, 0) as sba4ano,
			coalesce(subacao.sbaunt,0) as sbaunt,
			subacao.sbauntdsc,
			subacao.sba0ini,
			subacao.sba0fim,
			subacao.sba1ini,
			subacao.sba1fim,
			subacao.sba2ini,
			subacao.sba2fim,
			subacao.sba3ini,
			subacao.sba3fim,
			subacao.sba4ini,
			subacao.sba4fim,
			coalesce(u.unddsc,'') as unddsc,
			coalesce(f.frmdsc,'') as frmdsc,
			c.crtdsc,
			c.ctrpontuacao,
			f.frmid
		from
			cte.dimensao d
			inner join cte.areadimensao area ON area.dimid = d.dimid
			inner join cte.indicador i ON i.ardid = area.ardid
			inner join cte.pontuacao p ON p.indid = i.indid and p.ptostatus = '" . $ptostatus . "'
			inner join cte.criterio c ON c.crtid = p.crtid
			inner join cte.instrumentounidade iu ON iu.inuid = p.inuid
			inner join cte.acaoindicador acao ON acao.ptoid = p.ptoid
			inner join cte.subacaoindicador subacao ON subacao.aciid = acao.aciid
			inner join cte.unidademedida u on u.undid = subacao.undid
			inner join cte.formaexecucao f on f.frmid = subacao.frmid
			--left join cte.programa prg on prg.prgid = subacao.prgid
		where
			iu.inuid = '".$_SESSION['inuid']."' 
		order by
			d.dimcod,  
			area.ardcod,
			i.indcod, p.ptoid, acao.aciid, subacao.sbaid;
";

//dbg($sql,1 );
$dado = $db->carregar($sql);
$i=0;

$totalreg = count($dado);

$novaDimensao = $dado[$i]['dimid'];
$novaArea = $dado[$i]['ardid'];
$novoIndicador = $dado[$i]['indid'];
$novaAcao = $dado[$i]['aciid'];
$novasubAcao = $dado[$i]['sbaid'];

$totalGeralAno0 = 0;
$totalGeralAno1 = 0;
$totalGeralAno2 = 0;
$totalGeralAno3 = 0;
$totalGeralAno4 = 0;


?>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php if( !isset( $cabecalhoImprecao ) || $cabecalhoImprecao !== false ): ?>		
<thead>
	<th colspan="2" align="left">
	<?
	$sql = "select estdescricao from territorios.estado where estuf = '" . cte_pegarEstuf( $_SESSION['inuid'] ) . "'";
	$estado = $db->pegaUm($sql);
	?>
	<h1 class="notprint">PAR do Estado: <?=$estado?></h1>
	
		<table width="100%" bgcolor="#ffffff" border="0" cellpadding="0" cellspacing="0" class="notscreen" style="position: absolute; position: fixed;border-bottom: 1px solid;">
				<tr> 
					<td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
					<td height="20" nowrap>
						<b>SIMEC</b>- Sistema Integrado de Ministério da Educação<br>
						Ministério da Educação / SE - Secretaria Executiva<br>
						<b>.:: PAR Analítico do Estado:  <?=$estado?></b><br>	
					</td>
					<td height="20" align="right">
						Impresso por: <strong><?= $_SESSION['usunome']; ?></strong><br>
						Órgão: <?= $_SESSION['usuorgao']; ?><br>
						Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
					</td>
				</tr>
				<tr> 
					<td colspan="2">&nbsp;</td>
				</tr>
				</th>
			</table>
</thead>
<?php endif; ?>
<? 
while ( $i < $totalreg ) {

	$dimensao = $dado[$i]['dimid'];
		?>
					<tr>
						<td class="SubTituloDireita">
							<b>Dimensão</b>
						</td>
						<td class="" style="text-align:left">	
							<?=$dado[$i]['dimensao']; ?>
						</td>
					</tr>
					<?
					while ($dimensao == $novaDimensao)
					{
						if($i >= $totalreg ) break;

						$area = $dado[$i]['ardid'];

				?>
								<tr>
									<td class="SubTituloDireita">
										<b>Área</b>
									</td>
									<td class="" style="text-align:left">	
										<?=$dado[$i]['area']; ?>
									</td>
								</tr>
								
						
								<?

								while ($area == $novaArea )
								{
									if($i >= $totalreg ) break;

									$indicador = $dado[$i]['indid'];
									while ($indicador == $novoIndicador )
									{
										if($i >= $totalreg ) break;
										//print 'Indicador'.$indicador.'<br>';
										//print 'IndicadorNOVO'.$novoIndicador.'<br>';

												?>
													<tr>
														<td class="SubTituloDireita">
															<b>Indicador</b>
														</td>
														<td class="" style="text-align:left">	
															<?=$dado[$i]['indicador']; ?>
														</td>
													</tr>
													<tr>
														<td class="SubTituloDireita">
															<b>Critério / Pontuação</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ctrpontuacao'] . ' - '.$dado[$i]['crtdsc']; ?>
														</td>
													</tr>
													<tr>
														<td class="SubTituloDireita">
															<b>Justificativa</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ptojustificativa']; ?>
														</td>
													</tr>
													<?if( $dado[$i]['ptodemandaestadual']){?>
													<tr>
														<td class="SubTituloDireita">
															<b>Demanda para Rede Estadual</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ptodemandaestadual']; ?>
														</td>
													</tr>
													<?}?>
													<?if($dado[$i]['ptodemandamunicipal']){?>
													<tr>
														<td class="SubTituloDireita">
															<b>Demanda para Redes Municipais</b>
														</td>
														<td class="" style="text-align:left">	
															<?echo $dado[$i]['ptodemandamunicipal']; ?>
														</td>
													</tr>
													<?}?>
										
													<?

													$acao = $dado[$i]['aciid'];
													while ($acao == $novaAcao)
													{
														if($i >= $totalreg ) break;
														//print 'AçãoFORA'.$acao.'<br>';
														//print 'NovaAçãoFORA'.$novaAcao.'<br><p>';
											?>
																<tr>
																	<td class="SubTituloDireita">
																		<b>Ação</b>
																	</td>
																	<td class="" style="text-align:left">
																		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
																			<tr>
																				<td  class="SubTituloDireita">
																					Demanda:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['tipo'];?>
																				</td>
																			</tr>	
																			<tr>
																				<td  class="SubTituloDireita">
																					Descrição da Ação:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acidsc']; ?>
																				</td>
																			</tr>
																			<tr>
																				<td  class="SubTituloDireita">
																					Nome do Responsável:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acirpns']; ?>
																				</td>
																			</tr>	
																			<tr>
																				<td  class="SubTituloDireita">
																					Cargo do Responsável:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acicrg']; ?>
																				</td>
																			</tr>			 
																			<tr>
																				<td  class="SubTituloDireita">
																					Período Inicial:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acidtinicial']; ?>
																				</td>
																			</tr>
																			<tr>
																				<td  class="SubTituloDireita">
																					Período Final:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acidtfinal']; ?>
																				</td>
																			</tr>
																			<tr>
																				<td  class="SubTituloDireita">
																					Resultado Esperado:
																				</td>
																				
																				<td>
																					<?=$dado[$i]['acirstd']; ?>
																				</td>
																			</tr>
																		</table>	
																	</td>
																</tr>			
																<?
																$subacao = $dado[$i]['sbaid'];
																while ($acao == $novaAcao )
																{
																	mostra_subacao();
																	if($i >= $totalreg ) break;
																}
													}}

														?>	
												<tr>
													<td class="SubTituloDireita" >
													<b>Total Geral por Indicador</b>
													</td>
													<td>			
														<table class="listagem" width="100%">
															<thead>
																<th align="center">
																	<b>2007</b>
																</th>
																<th align="center">
																	<b>2008</b>
																</th>
																<th align="center">
																	<b>2009</b>
																</th>
																<th align="center">
																	<b>2010</b>
																</th>
																<th align="center">
																	<b>2011</b>
																</th>
																<th align="center">
																	<b>Total</b>
																</th>
															</thead>
															<tr>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno0,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno1,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno2,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno3,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno4,2,',','.');?>
																</td>
																<td align="right">
																	<?=number_format($totalGeralIndicadorAno0 + $totalGeralIndicadorAno1 + $totalGeralIndicadorAno2 + $totalGeralIndicadorAno3 + $totalGeralIndicadorAno4 ,2,',','.');?>
																</td>
															</tr>
														</table>
													</td>
												</tr>
												<?
												$totalGeralIndicadorAno0 = 0; 
												$totalGeralIndicadorAno1 = 0;
												$totalGeralIndicadorAno2 = 0;
												$totalGeralIndicadorAno3 = 0;
												$totalGeralIndicadorAno4 = 0;

								}
							?>
						<tr>
							<td class="SubTituloDireita" >
							<b>Total Geral por Área</b>
							</td>
							<td>			
								<table class="listagem" width="100%">
									<thead>
										<th align="center">
											<b>2007</b>
										</th>
										<th align="center">
											<b>2008</b>
										</th>
										<th align="center">
											<b>2009</b>
										</th>
										<th align="center">
											<b>2010</b>
										</th>
										<th align="center">
											<b>2011</b>
										</th>
										<th align="center">
											<b>Total</b>
										</th>
									</thead>
									<tr>
										<td align="right">
											<?=number_format($totalGeralAreaAno0,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno1,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno2,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno3,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno4,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralAreaAno0 + $totalGeralAreaAno1 + $totalGeralAreaAno2 + $totalGeralAreaAno3+ $totalGeralAreaAno4,2,',','.');?>
										</td>
									</tr>
								</table>
							</td>
						</tr>
						<?
						$totalGeralAreaAno0 = 0; 
						$totalGeralAreaAno1 = 0;
						$totalGeralAreaAno2 = 0;
						$totalGeralAreaAno3 = 0;
						$totalGeralAreaAno4 = 0;

					}
?>
		<tr>
							<td class="SubTituloDireita" >
							<b>Total Geral por Dimensão</b>
							</td>
							<td>			
								<table class="listagem" width="100%">
									<thead>
										<th align="center">
											<b>2007</b>
										</th>
										<th align="center">
											<b>2008</b>
										</th>
										<th align="center">
											<b>2009</b>
										</th>
										<th align="center">
											<b>2010</b>
										</th>
										<th align="center">
											<b>2011</b>
										</th>
										<th align="center">
											<b>Total</b>
										</th>
									</thead>
									<tr>
										<td align="right">
											<?=number_format($totalGeralDimensaAno0,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno1,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno2,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno3,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno4,2,',','.');?>
										</td>
										<td align="right">
											<?=number_format($totalGeralDimensaAno0 + $totalGeralDimensaAno1 + $totalGeralDimensaAno2 + $totalGeralDimensaAno3+ $totalGeralDimensaAno4,2,',','.');?>
										</td>
									</tr>
								</table>
							</td>
						</tr>
						<?
						$totalGeralDimensaAno0 = 0; 
						$totalGeralDimensaAno1 = 0;
						$totalGeralDimensaAno2 = 0;
						$totalGeralDimensaAno3 = 0;
						$totalGeralDimensaAno4 = 0;


	}?>	
<tr>
	<td class="SubTituloDireita"  colspan="2" style="text-align: center;">
	<b>Total Geral</b>
	</td>
</tr>
<tr>
	<td colspan="2" >
		<table class="listagem" width="100%">
			<thead>
				<th align="center">
					<b>2007</b>
				</th>
				<th align="center">
					<b>2008</b>
				</th>
				<th align="center">
					<b>2009</b>
				</th>
				<th align="center">
					<b>2010</b>
				</th>
				<th align="center">
					<b>2011</b>
				</th>
				<th align="center">
					<b>Total</b>
				</th>
			</thead>
			<tr>
				<td align="right">
					<?=number_format($totalGeralAno0,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno1,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno2,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno3,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno4,2,',','.');?>
				</td>
				<td align="right">
					<?=number_format($totalGeralAno0 + $totalGeralAno1 + $totalGeralAno2 + $totalGeralAno3 + $totalGeralAno4 ,2,',','.');?>
				</td>
			</tr>
		</table>
	</td>
</tr>
</table>
	
	
<?





function mostra_subacao(){

	global $dado, $i, $totalreg;
	global $total, $totalGeralAno0 ,$totalGeralAno1, $totalGeralAno2, $totalGeralAno3, $totalGeralAno4;
	global $totalGeralIndicadorAno0 , $totalGeralIndicadorAno1, $totalGeralIndicadorAno2, $totalGeralIndicadorAno3, $totalGeralIndicadorAno4;
	global $totalGeralAreaAno0 , $totalGeralAreaAno1, $totalGeralAreaAno2, $totalGeralAreaAno3, $totalGeralAreaAno4;
	global $totalGeralDimensaAno0 ,$totalGeralDimensaAno1, $totalGeralDimensaAno2, $totalGeralDimensaAno3, $totalGeralDimensaAno4;
	global $novaDimensao, $novaArea, $novoIndicador, $novaAcao, $novasubAcao;
		?>
					<tr>
						<td class="SubTituloDireita">
							<b>Sub-Ação</b>
						</td>
						<td class="" style="text-align:left">	
						
							<table  class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" >
								<tr>
									<td  class="SubTituloDireita">
										Descrição da Subação:
									</td>
									
									<td>
										<?=$dado[$i]['sbadsc']; ?>
									</td>
								</tr>									
								<tr>
									<td  class="SubTituloDireita">
										Estratégia de Implementação:
									</td>
									
									<td>
										<?=$dado[$i]['sbastgmpl']; ?>
									</td>
								</tr>
								<tr>
									<td  class="SubTituloDireita">
										Programa:
									</td>
									
									<td>
										<?=$dado[$i]['sbaprm']; ?>
									</td>
								</tr>
								<tr>
									<td  class="SubTituloDireita">
										Unidade de Medida:
									</td>
									
									<td>
										<?=$dado[$i]['unddsc']; ?>
									</td>
								</tr>
								<tr>
									<td  class="SubTituloDireita">
										Forma de Execução
									</td>
									
									<td>
										<?=$dado[$i]['frmdsc']; ?>
									</td>
								</tr>
								<tr>
									<td  class="SubTituloDireita">
										Instituição Parceira (se houver):
									</td>
									
									<td>
										<?=$dado[$i]['sbapcr']; ?>
									</td>
								</tr>
								<tr>
									<td  class="SubTituloDireita">
										Quantidades e Cronograma Físico
									</td>									
									<td>
										<table class="listagem" width="100%">
											<thead> 
												<th>
													&nbsp;	
												</th>	
												<th align="center">
														<b>2007</b>
												</th>											
												<th align="center">
														<b>2008</b>
												</th>																								
												<th align="center">
														<b>2009</b>
												</th>
												<th align="center">
														<b>2010</b>
												</th>
												<th align="center">
														<b>2011</b>
												</th>
												<th align="center">
													<b>Total</b>
												</th>
											</thead>											
											<tr>
												<td align="right">
													<b>Quantidades:</b>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sba0ano'],0); ?>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sba1ano'],0); ?>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sba2ano'],0); ?>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sba3ano'],0); ?>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sba4ano'],0); ?>
												</td>
												<td align="right">
													<?
													$total = 	
													$dado[$i]['sba0ano'] +
													$dado[$i]['sba1ano'] +
													$dado[$i]['sba2ano'] +
													$dado[$i]['sba3ano'] +
													$dado[$i]['sba4ano'];
													echo number_format($total,0);
													 ?>
												</td>
											</tr>											
											<tr>
												<td align="right">
													<b>Cronograma Físico:</b>
												</td>
												<td align="right">
													<?if(!Empty($dado[$i]['sba0ini'])) echo $dado[$i]['sba0ini'] . " até " ; ?>  <?=$dado[$i]['sba0fim']; ?>
												</td>
												<td align="right">
													<?if(!Empty($dado[$i]['sba1ini'])) echo $dado[$i]['sba1ini'] . " até " ; ?>  <?=$dado[$i]['sba1fim']; ?>
												</td>
												
												<td align="right">
													<?if(!Empty($dado[$i]['sba2ini'])) echo $dado[$i]['sba2ini'] . " até " ; ?>  <?=$dado[$i]['sba2fim']; ?>		
												</td>
												<td align="right">
													<?if(!Empty($dado[$i]['sba3ini'])) echo $dado[$i]['sba3ini'] . " até " ; ?> <?=$dado[$i]['sba3fim']; ?>
												</td>
												<td align="right">
													<?if(!Empty($dado[$i]['sba4ini'])) echo $dado[$i]['sba4ini'] . " até " ; ?> <?=$dado[$i]['sba4fim']; ?>
												</td>
												<td align="right">
													&nbsp;
												</td>
											</tr>											
										</table>

										<?php if ( $dado[$i]['sbaunt'] > 0 ) { ?>

										<table 	class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
											<tr>
												<td align="right">
													<b>Valor Unitário:</b>
												</td>
												<td align="right">
													<?=number_format($dado[$i]['sbaunt'],2,',','.');?>
												</td>
											</tr>
											<tr>
												<td align="right">
													<b>Detalhamento da Composição</br> do Valor Unitário:</b>  	
												</td>
												<td>
													<?=$dado[$i]['sbauntdsc']?>
												</td>
											</tr>
										</table>
										
										<table class="listagem" width="100%"  cellspacing="1" cellpadding="3" >
											<thead>
												<th>
													&nbsp;
												</th>
												<th align="center">
													<b>2007</b>
												</th>
												<th align="center">
													<b>2008</b>
												</th>
												<th align="center">
													<b>2009</b>
												</th>
												<th align="center">
													<b>2010</b>
												</th>
												<th align="center">
													<b>2011</b>
												</th>
												<th align="center">
													<b>Total</b>
												</th>
											</thead>
											<tr>
												<?
												$ano0 = $dado[$i]['sbaunt'] * $dado[$i]['sba0ano'];
												$ano1 = $dado[$i]['sbaunt'] * $dado[$i]['sba1ano'];
												$ano2 = $dado[$i]['sbaunt'] * $dado[$i]['sba2ano'];
												$ano3 = $dado[$i]['sbaunt'] * $dado[$i]['sba3ano'];
												$ano4 = $dado[$i]['sbaunt'] * $dado[$i]['sba4ano'];

												$total = $ano0 + $ano1 + $ano2 + $ano3 + $ano4;
					
												$totalGeralAno0 += $ano0;	
												$totalGeralAno1 += $ano1;
												$totalGeralAno2 += $ano2;
												$totalGeralAno3 += $ano3;
												$totalGeralAno4 += $ano4;

												$totalGeralIndicadorAno0 += $ano0;
												$totalGeralIndicadorAno1 += $ano1;
												$totalGeralIndicadorAno2 += $ano2;
												$totalGeralIndicadorAno3 += $ano3;
												$totalGeralIndicadorAno4 += $ano4;

												$totalGeralAreaAno0 += $ano0;
												$totalGeralAreaAno1 += $ano1;
												$totalGeralAreaAno2 += $ano2;
												$totalGeralAreaAno3 += $ano3;
												$totalGeralAreaAno4 += $ano4;

												$totalGeralDimensaAno0 += $ano0;
												$totalGeralDimensaAno1 += $ano1;
												$totalGeralDimensaAno2 += $ano2;
												$totalGeralDimensaAno3 += $ano3;
												$totalGeralDimensaAno4 += $ano4;


												 ?>
												<td align="right">
													<b>Valores Anuais:</b>
												</td>
												<td align="right">
													<?=number_format($ano0,2,',','.');?>
												</td>											
												<td align="right">
													<?=number_format($ano1,2,',','.');?>
												</td>
												<td align="right">
													<?=number_format($ano2,2,',','.');?>
												</td>
												<td align="right">
													<?=number_format($ano3,2,',','.');?>
												</td>
												<td align="right">
													<?=number_format($ano4,2,',','.');?>
												</td>
												<td align="right">
													<?=number_format($total,2,',','.') ?>
												</td>
											</tr>		
										</table>
										<?php }  ?>
																																												
									</td>
								</tr>
							</table>	
						</td>
					</tr>
					
				<?		$i++;			

				$novaDimensao = $dado[$i]['dimid'];
				$novaArea = $dado[$i]['ardid'];
				$novoIndicador = $dado[$i]['indid'];
				$novaAcao = $dado[$i]['aciid'];
				$novasubAcao = $dado[$i]['sbaid'];

}

?>
