<?php
ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once "../_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";


$_SESSION['usucpf'] = '';
$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


$sql = "SELECT nomeparlamentar, emenda, gnd, mod, fonte, cnpj, sum(total) as total, uo
		FROM carga.emenda_delator
        group by 
			nomeparlamentar, emenda, gnd, mod, 
			fonte, cnpj, uo";
		
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $v) {
	
	$etoid = $db->pegaUm("select etoid from emenda.emenda where emecod = '{$v['emenda']}' and emeano = '".date('Y')."'");
	
	$sql = "select 
			    ed.emdid
			from emenda.emenda e
				inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
			where
				e.emecod = '{$v['emenda']}'
				and ed.gndcod = '{$v['gnd']}'
				and ed.mapcod = '{$v['mod']}'
				and ed.foncod = '{$v['fonte']}'
			    and e.emeano = '".date('Y')."'";
	$emdid = $db->pegaUm($sql);
	
	$cnpj = str_ireplace(' ', '', $v['cnpj']);
	
	$sql = "SELECT enbid FROM emenda.entidadebeneficiada WHERE enbcnpj='".$cnpj."' and enbano = '".date('Y')."'";
	$enbid = $db->pegaUm($sql);
	//ver($sql,$enbid,$cnpj,d);
	$arrResp = carregarUnidadesFederais( $v['uo'] );
	$muncod = ($arrResp['muncod'] ? "'".$arrResp['muncod']."'": 'null');
	$estuf = ($arrResp['estuf'] ? "'".$arrResp['estuf']."'": 'null');
	
	if( empty($enbid) ){
		$sql = "INSERT INTO emenda.entidadebeneficiada(enbstatus, enbano, enbdataalteracao, enbnome, enbcnpj, muncod, estuf)
	    				VALUES ('A',
	    						'".date('Y')."',
	    						NOW(),
	    						'".$v['beneficiario']."',
	    						'".$v['cnpj']."',
	    						$muncod,
	    						$estuf) RETURNING enbid";
		$enbid = $db->pegaUm($sql);
	}
	
	$edeid = $db->pegaUm("select edeid from emenda.emendadetalheentidade where emdid = $emdid and edestatus = 'A' and enbid = $enbid and edestatus = 'A'");
	
	if( $etoid == 4 && !empty($arrResp['usucpf']) ){
		if( empty($edeid) ){
			$sql = "INSERT INTO emenda.emendadetalheentidade ( emdid, enbid, edevalor, edevalordisponivel, usucpfalteracao, ededataalteracao, edecpfresp, edenomerep, edemailresp, ededddresp, edetelresp, edestatus, ededisponivelpta )
					VALUES ( {$emdid}, {$enbid}, {$v["total"]}, {$v["total"]}, '{$_SESSION['usucpf']}', 'now()', '{$arrResp["usucpf"]}', '{$arrResp["nomeusuario"]}', '{$arrResp["usuemail"]}', '{$arrResp["dd"]}', '{$arrResp["fone"]}', 'A', 'S' )";
			$db->executar( $sql );
		} else {
			$sql = "UPDATE emenda.emendadetalheentidade SET enbid = {$enbid}, edevalor = {$v["total"]}, edevalordisponivel = {$v["total"]}, edestatus = 'A' WHERE edeid = {$edeid}";
			$db->executar($sql);
		}
		
		$boContra = $db->pegaUm("select count(enbid) from emenda.entbeneficiadacontrapartida where enbid = $enbid");
		if( $boContra < 1 ){
			$sql = "INSERT INTO emenda.entbeneficiadacontrapartida(enbid, ebcexercicio, ebccontrapartida)
					VALUES ($enbid, '".date('Y')."', 0)";
			$db->executar($sql);
		}
			
		$boUserResp = $db->pegaUm("select count(rpuid) from emenda.usuarioresponsabilidade where pflcod = 274 and usucpf = '{$arrResp["usucpf"]}' and enbid = $enbid and rpustatus = 'A'");
		if( $boUserResp < 1 ){
			$sql = "INSERT INTO emenda.usuarioresponsabilidade(pflcod, usucpf, enbid, rpustatus, rpudata_inc)
					VALUES (274, '{$arrResp["usucpf"]}', $enbid, 'A', now())";
			$db->executar($sql);
		}
		
		$boUserSis = $db->pegaUm("select count(u.usucpf) from seguranca.usuario_sistema u where u.usucpf = '{$arrResp["usucpf"]}' and sisid = 57");
		$boPerUser = $db->pegaUm("select count(p.usucpf) from seguranca.perfilusuario p where p.usucpf = '{$arrResp["usucpf"]}' and pflcod = 274");
			
		if( $boPerUser < 1 ){
			$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod)
					VALUES ('{$arrResp["usucpf"]}', 274)";
			$db->executar($sql);
		}
			
		if( $boUserSis < 1 ){
			$sql = "INSERT INTO seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
					VALUES ('{$arrResp["usucpf"]}', 57, 'A', 274, now(), 'A')";
			$db->executar($sql);
		} else {
			$sql = "UPDATE seguranca.usuario_sistema SET susstatus = 'A', suscod = 'A' WHERE usucpf = '{$arrResp["usucpf"]}' and sisid = 57";
			$db->executar($sql);
		}
	} else {
		if( empty($edeid) ){
			$sql = "INSERT INTO emenda.emendadetalheentidade ( emdid, enbid, edevalor, edevalordisponivel, usucpfalteracao, ededataalteracao, edestatus, ededisponivelpta )
					VALUES ( {$emdid}, {$enbid}, {$v["total"]}, {$v["total"]}, '{$_SESSION['usucpf']}', 'now()', 'A', 'S' )";
			$db->executar( $sql );
		} else {
			$sql = "UPDATE emenda.emendadetalheentidade SET enbid = {$enbid}, edevalor = {$v["total"]}, edevalordisponivel = {$v["total"]}, edestatus = 'A' WHERE edeid = {$edeid}";
			$db->executar($sql);
		}
	}
	$db->commit();
}

function carregarUnidadesFederais( $unicod ){
	global $db;
	
	$sql = "select
				entnumcpfcnpj,
			    entnome,
			    estuf,
			    muncod,
			    usucpf,
			    nomeusuario,
			    dd,
			    fone,
			    cargo,
			    usuemail,
			    entid,
			    unicod
			from(
			    SELECT distinct
			        e.entnumcpfcnpj,
			        e.entnome,
			        mun.estuf,
			        mun.muncod,
			        usu.usucpf, 
			        usu.usunome as nomeusuario,		   			   		
			        usu.usufoneddd as dd,
			        usu.usufonenum as fone, 
			        COALESCE(car.cardsc,'')||' / '||COALESCE(usu.usufuncao,'') as cargo,
			        usu.usuemail,
			        e.entid,
			        e.entunicod as unicod
			    FROM  
			        entidade.entidade e 
			        inner join entidade.funcaoentidade fe on fe.entid = e.entid and fe.fuestatus = 'A' 
			        inner join academico.usuarioresponsabilidade ur on ur.entid = e.entid and ur.rpustatus = 'A'
			        inner join seguranca.usuario usu on usu.usucpf = ur.usucpf
			        inner join seguranca.perfilusuario pfu on usu.usucpf = pfu.usucpf
			        inner join seguranca.usuario_sistema uss on usu.usucpf = uss.usucpf
			        left join  public.cargo car              on car.carid = usu.carid
			        left join entidade.endereco en 
			            inner join territorios.municipio mun on mun.muncod = en.muncod
			        on en.entid = e.entid --and en.endstatus = 'A'
			    WHERE
			        /**e.entunicod = '26426'
			        and*/ e.entstatus = 'A'
			        and fe.funid = 12
			        and ur.pflcod = 526
			        and uss.sisid = '56' 
			        and pfu.pflcod = 526
			        and uss.suscod = 'A'
			        and e.entnumcpfcnpj is not null
			    union all                
			    select 
			        entnumcpfcnpj,
			        entnome,
			        estuf,
			        muncod,
			        usucpf,
			        nomeusuario,
			        dd,
			        fone,
			        case when cargo = '' then 'Outros' else cargo end as cargo,
			        usuemail,
			        entid,
			        unicod
			    from(
			        SELECT
			            case when ent.entnumcpfcnpj is null then ung.ungcnpj else ent.entnumcpfcnpj end as entnumcpfcnpj,
			            ent.entnome,
			            mun.estuf,
			            mun.muncod,
			            rl.cpf as usucpf,
			            rl.nome as nomeusuario,
			            case when rl.email = '-' then usu.usuemail else rl.email end as usuemail,
			            usu.usufoneddd as dd,
			            usu.usufonenum as fone,
			            case when rl.funcao is null then 
			                case when usu.usufuncao is null then 'Outros' else usu.usufuncao end
			            else 
			                case when rl.funcao is null then 'Outros' else rl.funcao end
			            end as cargo,
			            ent.entid,
			            ung.unicod
			        FROM
			            elabrev.representantelegal rl
			            inner join public.unidadegestora ung ON ung.ungcod = rl.ug
			            inner join entidade.entidade ent on ent.entunicod = ung.unicod
			            inner join entidade.funcaoentidade fe on fe.entid = ent.entid
			            left join entidade.endereco en 
			                inner join territorios.municipio mun on mun.muncod = en.muncod
			            on en.entid = ent.entid --and en.endstatus = 'A'
			    			        
			            left join seguranca.usuario usu on usu.usucpf = rl.cpf
			            left join public.cargo car on car.carid = usu.carid
			        WHERE
			            rl.status = 'A'
			            --and ung.unicod = '26426'
			            and fe.funid in (select funid from entidade.funcao f where (f.fundsc ilike '%univer%' or f.fundsc ilike '%federal%') )
			    ) as foo
			    where
			        entnumcpfcnpj is not null
			) as foo
			where
				unicod = '$unicod'";
	
	$arrDados = $db->pegaLinha($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	return $arrDados;
}