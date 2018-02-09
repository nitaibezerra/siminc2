<?php
header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sisfor/_constantes.php";
require_once APPRAIZ . "www/sisfor/_funcoes.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();



   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

if(date('w')==1) :

$sql = "select distinct c.curnome as nome, c.curemail as email,
						case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
						     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
						     when s.ocuid is not null then oc.ocunome 
						     when s.oatid is not null then oatnome end as curso, c.curid, s.sifid
		
		
		from sisfor.cursista c 
		inner join sisfor.cursistacurso cc on cc.curid = c.curid 
		inner join sisfor.sisfor s on s.sifid = cc.sifid and s.sifexecucaosisfor=true
		inner join workflow.documento d on d.docid = s.docidprojeto and d.esdid=1187 
		left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
		left join catalogocurso2014.curso cur on cur.curid = ieo.curid
		left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
		left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
		left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
		left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
		left join seguranca.usuario usu on usu.usucpf = s.usucpf
		left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
		left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
		left join sisfor.outraatividade oat on oat.oatid = s.oatid 
		left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid
		where c.curstatus='A' and cucstatus='A' and (
		(curescolaridade is null or currede is null or curcontratacao is null or cursexo is null or curdatanascimento is null or curraca is null or curdeficiencia is null or curinep is null or curfuncao is null) or
		(cc.muncod is null))";

$foo = $db->carregar($sql);

if($foo[0]) {
	foreach($foo as $f) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISFOR - Atualização das informações complementares do cursistas";
		
		$mensagem->AddAddress( $f['email'], $f['nome'] );
		
		$html =  '<p>Prezado(a) '.$f['nome'].'</p>';
		$html .= '<p>Estamos solicitando aos cursistas que acessem o link abaixo e atualizem os dados complementares do curso : <b>'.$f['curso'].'</b></p>';
		$html .= '<p>O preenchimento não é obrigatório para o cursista, mas caso o cursista não efetue a atualização, o coordenador do curso deverá cadastrar essas informações de todos.</p>';
		$html .= '<p>A atualização é simples e rápida, acesse o link : <a href="http://simec.mec.gov.br/sisfor/sisfor_atualizar_cursistas.php?curid='.base64_encode($f['curid']).'&sifid='.base64_encode($f['sifid']).'" target="_blank">http://simec.mec.gov.br/sisfor/sisfor_atualizar_cursistas.php?curid='.base64_encode($f['curid']).'&sifid='.base64_encode($f['sifid']).'</a></p>';
		$html .= '<p>Att,<br>Equipe MEC</p>';
			
		$mensagem->Body = $html;
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Enviado : ".$resp."<br>";
	}
}

endif;






if(date("d")=='01') :

$sql = "SELECT 
						'<span style=font-size:x-small><b>'||CASE WHEN a.alptipo='orcamento' THEN 'Orçamento do curso' 
							 WHEN a.alptipo='bolsas' THEN 'Qtd de bolsas por perfil'
							 WHEN a.alptipo='meta' THEN 'Meta do curso' 
							 WHEN a.alptipo='vigencia' THEN 'Vigência do curso'
							 END||'</b></span>' as tipo,
						'<span style=font-size:x-small>'||uu.uniabrev||' - '||uu.unidsc||'</span>' as universidade, 
						'<span style=font-size:x-small>'||case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
						     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
						     when s.ocuid is not null then oc.ocunome 
						     when s.oatid is not null then oatnome end||'</span>' as curso,
						case when s.ieoid is not null then cor.coordid
					     	 when s.cnvid is not null then cor2.coordid
					     	 when s.ocuid is not null then cor3.coordid
					     	 when s.oatid is not null then cor4.coordid end as coordid,
						'<span style=font-size:x-small>'||u.usunome||' ( '||to_char(alpdtsolicitou,'dd/mm/YYYY HH24:MI')||' )</span>' as solicitacao,
					   coalesce(alpjustificativa,'-') as justificativa,
					   '<span style=font-size:x-small>'||CASE WHEN alpautorizado='1' THEN 'EM ANÁLISE {$param}'
							WHEN alpautorizado='2' THEN 'AUTORIZADO'
							WHEN alpautorizado='3' THEN 'RECUSADO' END||CASE WHEN alpdtautorizou IS NOT NULL THEN ' ( '||u2.usunome||' - '||to_char(alpdtautorizou,'dd/mm/YYYY HH24:MI')||' )' ELSE '' END||'</span>' as situacao
				FROM sisfor.alterarprojeto a 
				INNER JOIN seguranca.usuario u ON u.usucpf = a.usucpfsolicitou 
				LEFT JOIN seguranca.usuario u2 ON u2.usucpf = a.usucpfautorizou  
				LEFT JOIN sisfor.sisfor s ON s.sifid = a.sifid and s.sifexecucaosisfor=true
				LEFT JOIN public.unidade uu on uu.unicod = s.unicod
				left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
				left join catalogocurso2014.curso cur on cur.curid = ieo.curid
				left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
				left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
				left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
				left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
				left join seguranca.usuario usu on usu.usucpf = s.usucpf
				left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
				left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
				left join sisfor.outraatividade oat on oat.oatid = s.oatid 
				left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
				
				WHERE alpautorizado='1' 
				ORDER BY a.alpdtsolicitou";

$foo = $db->carregar($sql);

if($foo[0]) {
	foreach($foo as $f) {
		$_conteudoemail[$f['coordid']][] = $f;
	}
}

if($_conteudoemail) {
	foreach($_conteudoemail as $coordid => $ar1) {
		$sql = "SELECT DISTINCT u.usunome as nome, u.usuemail as email, u.ususenha as senha FROM sisfor.usuarioresponsabilidade r
				INNER JOIN seguranca.usuario u ON u.usucpf = r.usucpf
				INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf AND us.sisid=".SIS_SISFOR."
				INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf AND pu.pflcod = r.pflcod
				WHERE r.rpustatus='A' AND r.pflcod IN(".PFL_COORDENADOR_MEC.",".PFL_DIRETOR_MEC.") AND r.coordid='".$coordid."'";

		$dest = $db->carregar($sql);

		$coordenacao = $db->pegaUm("SELECT coorddesc ||' ( '||coordsigla||' )' as coordenacao FROM catalogocurso2014.coordenacao WHERE coordid='{$coordid}'");

		$html  = '<p>Prezado {nome},</p>';
		$html .= '<p>Identificamos que existem solicitações de <b>MUDANÇAS DE PROJETOS PENDENTES</b> no Sistema de Gestão e Monitoramento da Formação Continuada do MEC (SISFOR) na <b>'.$coordenacao.'</b>  aguardando análise do MEC (Coordenadores e/ou Diretores). Seguem a lista das solicitações:</p>';
		$html .= '<table width=100%>';

		$html .= '<tr>';
		$html .= '<td style=font-size:x-small; align=center><b>IES</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Curso</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Tipo de alteração</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Solicitado por:</b></td>';
		$html .= '</tr>';


		foreach($ar1 as $c) {
			$html .= '<tr>';
			$html .= '<td style=font-size:x-small;>'.$c['universidade'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['curso'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['tipo'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['solicitacao'].'</td>';
			$html .= '</tr>';
		}

		$html .= '</table>';
		$html .= '<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha {senha}. O caminho é SISFOR => Principal => MEC => Alteração de projeto</p>';

		if($dest[0]) {
			foreach($dest as $de) {

				$mensagem = new PHPMailer();
				$mensagem->persistencia = $db;

				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= SIGLA_SISTEMA;
				$mensagem->From 		= "noreply@mec.gov.br";
				$mensagem->Subject 		= SIGLA_SISTEMA. " - SISFOR - Análise das solicitações de mudança de projeto pelo MEC";

				$mensagem->AddAddress( $de['email'], $de['nome'] );

				$htmlf = str_replace(array('{nome}','{senha}'),array($de['nome'],md5_decrypt_senha( $de['senha'], '' )),$html);
					
				$mensagem->Body = $htmlf;
				$mensagem->IsHTML( true );
				$resp = $mensagem->Send();
				echo "Enviado : ".$resp."<br>";

			}
		}

	}
}




$sql = "select
				u.uniabrev ||' - '|| unidsc as uninome,
				u.unicod,
            	case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc
				     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc
				     when s.ocuid is not null then oc.ocunome
				     when s.oatid is not null then oatnome end as curdesc,
			    iusnome as iusnome,
                to_char((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date,'mm/YYYY') as mesanoini,
				esd.esdid,
			  	esd.esddsc as esddsc,
				(select count(*) as total from sisfor.mensario m
				 inner join sisfor.tipoperfil t on t.tpeid = m.tpeid
				 inner join sisfor.mensarioavaliacoes ma on ma.menid = m.menid
				 where t.sifid = s.sifid and m.fpbid = f.fpbid and mavatividadesrealizadas='A') as qtdbolsasaptas,
				case when s.ieoid is not null then cor.coordid
			     	 when s.cnvid is not null then cor2.coordid
			     	 when s.ocuid is not null then cor3.coordid
			     	 when s.oatid is not null then cor4.coordid end as secretaria,
				case when s.ieoid is not null then cor.coordid
			     	 when s.cnvid is not null then cor2.coordid
			     	 when s.ocuid is not null then cor3.coordid
			     	 when s.oatid is not null then cor4.coordid end as coordid,
				to_char(hst.htddata, 'dd/mm/YYYY HH24:MI') as hstdata
        from sisfor.sisfor s
        		inner join workflow.documento dprojeto on dprojeto.docid = s.docidprojeto and dprojeto.esdid=1187
                inner join public.unidade u on u.unicod = s.unicod and u.unitpocod = s.unitpocod
                left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
                left join catalogocurso2014.curso cur on cur.curid = ieo.curid AND cur.curstatus = 'A'
                left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
                left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
                left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid AND cur2.curstatus = 'A'
                left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
                left join seguranca.usuario usu on usu.usucpf = s.usucpf
                left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
                left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid
				left join sisfor.outraatividade oat on oat.oatid = s.oatid
				left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid
                inner join sisfor.folhapagamentoprojeto fpp on s.sifid = fpp.sifid
                INNER JOIN sisfor.folhapagamento f ON f.fpbid = fpp.fpbid
                inner join workflow.documento doc on doc.docid = fpp.docid
				inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
				left join workflow.historicodocumento hst on hst.hstid = doc.hstid
				left join seguranca.usuario uu on uu.usucpf = hst.usucpf
                INNER JOIN sisfor.identificacaousuario i ON i.iuscpf=s.usucpf
            where doc.esdid IN('".ESD_ANALISE_MEC."') and s.sifexecucaosisfor=true
			order by uninome, curdesc, (fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date";

$foo = $db->carregar($sql);

if($foo[0]) {
	foreach($foo as $f) {
		$_conteudoemail[$f['coordid']][] = $f;
	}
}

if($_conteudoemail) {
	foreach($_conteudoemail as $coordid => $ar1) {
		$sql = "SELECT DISTINCT u.usunome as nome, u.usuemail as email, u.ususenha as senha FROM sisfor.usuarioresponsabilidade r 
				INNER JOIN seguranca.usuario u ON u.usucpf = r.usucpf 
				INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf AND us.sisid=".SIS_SISFOR." 
				INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf AND pu.pflcod = r.pflcod 
				WHERE r.rpustatus='A' AND r.pflcod IN(".PFL_COORDENADOR_MEC.",".PFL_DIRETOR_MEC.") AND r.coordid='".$coordid."'";
		
		$dest = $db->carregar($sql);
		
		$coordenacao = $db->pegaUm("SELECT coorddesc ||' ( '||coordsigla||' )' as coordenacao FROM catalogocurso2014.coordenacao WHERE coordid='{$coordid}'");
		
		$html  = '<p>Prezado {nome},</p>';
		$html .= '<p>Identificamos que existem solicitações de pagamentos <b>PENDENTES</b> no Sistema de Gestão e Monitoramento da Formação Continuada do MEC (SISFOR) na <b>'.$coordenacao.'</b>  aguardando análise do MEC (Coordenadores e/ou Diretores). Seguem a lista das solicitações:</p>';
		$html .= '<table width=100%>';
		
		$html .= '<tr>';
		$html .= '<td style=font-size:x-small; align=center><b>IES</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Curso</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Coordenador do curso</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Período</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Aguardando análise desde:</b></td>';
		$html .= '</tr>';
		
		
		foreach($ar1 as $c) {
			$html .= '<tr>';
			$html .= '<td style=font-size:x-small;>'.$c['uninome'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['curdesc'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['iusnome'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['mesanoini'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['hstdata'].'</td>';
			$html .= '</tr>';
		}
		
		$html .= '</table>';
		$html .= '<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha {senha}. O caminho é SISFOR => Principal => MEC => Análise das avaliações</p>';
		
		if($dest[0]) {
			foreach($dest as $de) {
				
				$mensagem = new PHPMailer();
				$mensagem->persistencia = $db;
				
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= SIGLA_SISTEMA;
				$mensagem->From 		= "noreply@mec.gov.br";
				$mensagem->Subject 		= SIGLA_SISTEMA. " - SISFOR - Análise das avaliações pelo MEC";
				
				$mensagem->AddAddress( $de['email'], $de['nome'] );
				
				$htmlf = str_replace(array('{nome}','{senha}'),array($de['nome'],md5_decrypt_senha( $de['senha'], '' )),$html);
					
				$mensagem->Body = $htmlf;
				$mensagem->IsHTML( true );
				$resp = $mensagem->Send();
				echo "Enviado : ".$resp."<br>";
				
			}
		}
		
	}
}


$sql = "select
				u.uniabrev ||' - '|| unidsc as uninome,
				u.unicod,
            	case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc
				     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc
				     when s.ocuid is not null then oc.ocunome
				     when s.oatid is not null then oatnome end as curdesc,
			    iusnome as iusnome,
                to_char((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date,'mm/YYYY') as mesanoini,
				esd.esdid,
			  	esd.esddsc as esddsc,
				(select count(*) as total from sisfor.mensario m
				 inner join sisfor.tipoperfil t on t.tpeid = m.tpeid
				 inner join sisfor.mensarioavaliacoes ma on ma.menid = m.menid
				 where t.sifid = s.sifid and m.fpbid = f.fpbid and mavatividadesrealizadas='A') as qtdbolsasaptas,
				case when s.ieoid is not null then cor.coordsigla
			     	 when s.cnvid is not null then cor2.coordsigla
			     	 when s.ocuid is not null then cor3.coordsigla
			     	 when s.oatid is not null then cor4.coordsigla end as secretaria,
				case when s.ieoid is not null then cor.coordid
			     	 when s.cnvid is not null then cor2.coordid
			     	 when s.ocuid is not null then cor3.coordid
			     	 when s.oatid is not null then cor4.coordid end as coordid,
				to_char(hst.htddata, 'dd/mm/YYYY HH24:MI') as hstdata
        from sisfor.sisfor s
        		inner join workflow.documento dprojeto on dprojeto.docid = s.docidprojeto and dprojeto.esdid=1187
                inner join public.unidade u on u.unicod = s.unicod and u.unitpocod = s.unitpocod
                left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
                left join catalogocurso2014.curso cur on cur.curid = ieo.curid AND cur.curstatus = 'A'
                left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
                left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
                left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid AND cur2.curstatus = 'A'
                left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
                left join seguranca.usuario usu on usu.usucpf = s.usucpf
                left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
                left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid
				left join sisfor.outraatividade oat on oat.oatid = s.oatid
				left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid
                inner join sisfor.folhapagamentoprojeto fpp on s.sifid = fpp.sifid
                INNER JOIN sisfor.folhapagamento f ON f.fpbid = fpp.fpbid
                inner join workflow.documento doc on doc.docid = fpp.docid
				inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
				left join workflow.historicodocumento hst on hst.hstid = doc.hstid
				left join seguranca.usuario uu on uu.usucpf = hst.usucpf
                INNER JOIN sisfor.identificacaousuario i ON i.iuscpf=s.usucpf
            where doc.esdid IN('".ESD_ANALISE_COORDENADORINSTITUCIONAL."') and s.sifexecucaosisfor=true 
			order by uninome, curdesc, (fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date";

$foo = $db->carregar($sql);

if($foo[0]) {
	foreach($foo as $f) {
		$_conteudoemail[$f['unicod']][] = $f;
	}
}

if($_conteudoemail) {
	foreach($_conteudoemail as $unicod => $ar1) {
		$sql = "SELECT DISTINCT u.usunome as nome, u.usuemail as email, u.ususenha as senha FROM sisfor.sisfories r
				INNER JOIN seguranca.usuario u ON u.usucpf = r.usucpf
				WHERE r.unicod='".$unicod."'";

		$dest = $db->carregar($sql);

		$universidade = $db->pegaUm("SELECT uniabrev||' - '||unidsc as universidade FROM public.unidade WHERE unicod='{$unicod}'");

		$html  = '<p>Prezado {nome},</p>';
		$html .= '<p>Identificamos que existem solicitações de pagamentos <b>PENDENTES</b> no Sistema de Gestão e Monitoramento da Formação Continuada do MEC (SISFOR) na  IES : <b>'.$universidade.'</b>  aguardando análise do Coordenador Institucional. Seguem a lista das solicitações:</p>';
		$html .= '<table width=100%>';

		$html .= '<tr>';
		$html .= '<td style=font-size:x-small; align=center><b>IES</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Diretoria</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Curso</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Coordenador do curso</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Período</b></td>';
		$html .= '<td style=font-size:x-small; align=center><b>Aguardando análise desde:</b></td>';
		$html .= '</tr>';


		foreach($ar1 as $c) {
			$html .= '<tr>';
			$html .= '<td style=font-size:x-small;>'.$c['uninome'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['secretaria'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['curdesc'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['iusnome'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['mesanoini'].'</td>';
			$html .= '<td style=font-size:x-small;>'.$c['hstdata'].'</td>';
			$html .= '</tr>';
		}

		$html .= '</table>';

		$html .= '<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha {senha}. O caminho é SISFOR => Principal => COMFOR => Análise das avaliações</p>';

		if($dest[0]) {
			foreach($dest as $de) {
				
				$mensagem = new PHPMailer();
				$mensagem->persistencia = $db;
				
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= SIGLA_SISTEMA;
				$mensagem->From 		= "noreply@mec.gov.br";
				$mensagem->Subject 		= SIGLA_SISTEMA. " - SISFOR - Análise das avaliações pelo MEC";
				
				$mensagem->AddAddress( $de['email'], $de['nome'] );
				
				$htmlf = str_replace(array('{nome}','{senha}'),array($de['nome'],md5_decrypt_senha( $de['senha'], '' )),$html);
					
				$mensagem->Body = $htmlf;
				$mensagem->IsHTML( true );
				$resp = $mensagem->Send();
				echo $resp."<br>";
				
			}
		}

	}
}

endif;



$sql = "select 
i.iusnome as nome, 
i.iusemailprincipal as email, 
uu.ususenha as senha, 
fp.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as periodo,
case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
     when s.ocuid is not null then oc.ocunome end as curso



from sisfor.identificacaousuario i 
inner join sisfor.tipoperfil t on t.iusd = i.iusd and t.pflcod in(1195,1105) 
inner join seguranca.usuario uu on uu.usucpf = i.iuscpf
inner join sisfor.sisfor s on s.sifid = t.sifid and s.sifexecucaosisfor=true

left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid 
left join catalogocurso2014.curso cur on cur.curid = ieo.curid 
left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid 
left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid 
left join sisfor.outrocurso oc on oc.ocuid = s.ocuid 


inner join workflow.documento d on d.docid = s.docidprojeto 
inner join sisfor.folhapagamentoprojeto fp on fp.sifid = s.sifid 
inner join sisfor.folhapagamento ff on ff.fpbid = fp.fpbid 
inner join public.meses m ON m.mescod::integer = ff.fpbmesreferencia
left join workflow.documento d2 on d2.docid = fp.docid 
where d.esdid=1187 and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd') and (d2.esdid=1206 or d2.esdid is null) 
order by nome, curso, periodo";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISFOR - Avaliação da equipe";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Informamos que seu cadastro ja esta liberado no SIMEC, e é fundamental que você faça avaliações sobre membros do curso <b>{$foo['curso']}</b>. Verificamos que você não fez a avaliação do período de referência: <b>".$foo['periodo']."</b></p>
		 <p>Para fazer a avaliação, acesse a aba de Execução e clique em Avaliar Equipe. Em seguida selecione as opções e aperte o botão 'Salvar'.</p>
		 <p>Em seguida no ícone 'Enviar para análise'. Este passo é muito importante para que a avaliação feita possa ser analisada pelo MEC.</p>
		 <p>Equipe SISFOR</p>
		 <br/><br/>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>";
				$mensagem->IsHTML( true );
				$resp = $mensagem->Send();
				echo "Avaliação da equipe<br>";
	}
}


/*
 * ALERTANDO TODOS OS PERFIS COM ACESSO AO SISPACTO DE PREENCHER O TERMO DE COMPROMISSO
*/

$sql = "select distinct i.iusnome as nome, i.iusemailprincipal as email, uu.ususenha as senha from sisfor.identificacaousuario i 
		inner join sisfor.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.sisid=177 
		inner join seguranca.usuario uu on uu.usucpf = i.iuscpf 
inner join sisfor.sisfor s on s.sifid = t.sifid and s.sifexecucaosisfor=true 
inner join workflow.documento d on d.docid = s.docidprojeto 
		where us.suscod='A' and uu.suscod='A' and i.iustermocompromisso is null and d.esdid=1187 
order by i.iusnome";


$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISFOR -  Preenchimento dos dados cadastrais";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Informamos que seu acesso ja esta liberado no SIMEC. Solicitamos que acesse o sistema e preencha os dados solicitados para o recebimento da bolsa.</p>
		 <p>Equipe SISFOR</p>
		 <br/><br/>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>
				";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Preenchimento de dados _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sisfor_enviaremails_alertas.php'";
$db->executar($sql);
$db->commit();


$db->close();


?>