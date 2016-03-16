<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

//$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if( $_REQUEST['req'] == 'deleta' ){ 
	deleteEntidade();
	exit();
}

$sql = "select 
			ee.emecod, 
			ed.emdid,
		    ed.emdvalor, 
		    f.unicod, 
			u.unidsc
		from 
			emenda.emenda ee
            inner join emenda.autor a on a.autid = ee.autid
		    inner join emenda.emendadetalhe ed on ed.emeid = ee.emeid
		    inner join emenda.v_funcionalprogramatica f on f.acaid = ee.acaid
            inner join public.unidade u on u.unicod = f.unicod
		where 
			ee.emeano = '".date('Y')."' and ee.etoid = 4
		    and f.prgano = '".date('Y')."' and f.acastatus = 'A'
            and a.autnome not ilike '%bancada%'
		    and ed.emdid in (select emdid from emenda.emendadetalheentidade where edestatus = 'A')";

/*$sql = "select distinct 
		    en.emecod,
		    emd.emdid,
		    emd.emdvalor,
		    u.unicod,
		    u.unidsc
		from public.unidade u
			inner join entidade.entidade e on e.entunicod = u.unicod and e.entstatus = 'A'
			inner join entidade.funcaoentidade f on f.entid = e.entid and funid in ( 12, 10, 16, 56 )
			inner join emenda.v_funcionalprogramatica  v on v.unicod = u.unicod and v.prgano = '".date('Y')."'
			inner join emenda.emenda en on en.acaid = v.acaid and emeano = '".date('Y')."'
		    inner join emenda.autor a on a.autid = en.autid
		    inner join emenda.emendadetalhe emd on emd.emeid = en.emeid
		WHERE en.emeid in (select emeid from emenda.v_emendadetalheentidade where emeano = '".date('Y')."')
			and e.entnumcpfcnpj is not null
			and en.etoid = 4
		    and a.autnome not ilike '%bancada%'";*/

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();
//ver($sql, sizeof($arrDados), d);
$totalAtualizada = 0;
$emendaNaoAtualizada = array();
$emendaAtualizada = array();
foreach ($arrDados as $v) {
	$arrResp = carregarUnidadesFederais( $v['unicod'] );
	//ver($arrResp,d);
	//if( empty($uniAcademico) ) $uniElabrev = carregarUnidadeElabrev( $v['unicod'] );
	
	if( empty($arrResp) ){
		array_push($emendaNaoAtualizada, array('emecod' => $v['emecod'], 'unicod' => $v['unicod'], 'unidsc'=>$v['unidsc']) );
	} else {
		/*if( !empty($uniAcademico)){
			$arrResp = $uniAcademico; 
		} else {
			$arrResp = $uniElabrev;
		}*/
		
		$enbid = $db->pegaUm("SELECT enbid FROM emenda.entidadebeneficiada WHERE enbcnpj='".$arrResp['entnumcpfcnpj']."' and enbano = '".date('Y')."'");
		
		/* if( empty($enbid) ){
			$sql = "INSERT INTO emenda.entidadebeneficiada(enbstatus, enbano, enbdataalteracao, enbnome, enbcnpj, muncod, estuf)
	    				VALUES ('A', '".date('Y')."', NOW(), '".$arrResp['entnome']."', '".$arrResp['entnumcpfcnpj']."', '".(integer)$arrResp['muncod']."', '".$arrResp['estuf']."') RETURNING enbid";
			$enbid = $db->pegaUm($sql);
		} */ 
		if( !empty($enbid) ){
			
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
			$edeid = $db->pegaUm("select edeid from emenda.emendadetalheentidade where emdid = {$v["emdid"]} and enbid = $enbid and edestatus = 'A'");
			if( empty($edeid) ){
				array_push($emendaNaoAtualizada, array('emecod' => $v['emecod'], 'unicod' => $v['unicod'], 'unidsc'=>$v['unidsc']) );
				/* $sql = "INSERT INTO emenda.emendadetalheentidade ( emdid, enbid, edevalor, usucpfalteracao, ededataalteracao, edecpfresp, edenomerep, edemailresp, ededddresp, edetelresp, edestatus, ededisponivelpta )
						VALUES ( {$v["emdid"]}, {$enbid}, '{$v["emdvalor"]}', '', 'now()', '{$arrResp["usucpf"]}', '{$arrResp["nomeusuario"]}', '{$arrResp["usuemail"]}', '{$arrResp["dd"]}', '{$arrResp["fone"]}', 'A', 'S' ) RETURNING edeid";
				$edeid = $db->pegaUm( $sql ); */
			} else {
				$sql = "UPDATE emenda.emendadetalheentidade SET 
							edecpfresp = '{$arrResp["usucpf"]}',
							edenomerep = '{$arrResp["nomeusuario"]}',
							edemailresp = '{$arrResp["usuemail"]}',
							ededddresp = '{$arrResp["dd"]}', 
							edetelresp = '{$arrResp["fone"]}',
							ededisponivelpta = 'S'
						WHERE edeid = $edeid";
				$db->executar($sql);
				
				array_push($emendaAtualizada, array('emecod' => $v['emecod'], 'unicod' => $v['unicod'], 'unidsc'=>$v['unidsc']) );
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
			$db->commit();
			
			$totalAtualizada++;
		} else {
			array_push($emendaNaoAtualizada, array('emecod' => $v['emecod'], 'unicod' => $v['unicod'], 'unidsc'=>$v['unidsc']) );
		}
	}
}
?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>Recursos</b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%"><b>Total de Emendas V1:</b></td>
		<td><?=sizeof($arrDados); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%"><b>Total de Emendas Atualizadas:</b></td>
		<td><?=$totalAtualizada; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>Emendas Atualizadas</b></td>
	</tr>
	<tr>
		<td colspan="2">
		<?
		$cabecalho = array('Emenda', 'Unicod', 'Unidade');
		$db->monta_lista_simples($emendaAtualizada, $cabecalho, 100000, 1, 'N', '100%', '', true, '', '', true);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>Emendas não Atualizadas</b></td>
	</tr>
	<tr>
		<td colspan="2">
		<?		
		$cabecalho = array('Emenda', 'Unicod', 'Unidade');
		$db->monta_lista_simples($emendaNaoAtualizada, $cabecalho, 100000, 1, 'N', '100%', '', true, '', '', true);
		?>
		</td>
	</tr>
</table>
<?

function deleteEntidade(){
	global $db;
	
	$sql = "select distinct
				ee.emecod, 
				ed.emdid,
			    ed.emdvalor, 
			    f.unicod, 
				u.unidsc,
	            ede.edeid
			from 
				emenda.emenda ee
			    inner join emenda.emendadetalhe ed on ed.emeid = ee.emeid
			    inner join emenda.v_funcionalprogramatica f on f.acaid = ee.acaid
	            inner join public.unidade u on u.unicod = f.unicod
	            inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid
			where 
				ee.emeano = '".date('Y')."' and ee.etoid = 4
			    and f.prgano = '".date('Y')."' and f.acastatus = 'A'
	            and f.unicod in (select entunicod from entidade.entidade e 
	                                inner join entidade.funcaoentidade fe on fe.entid = e.entid and fe.fuestatus = 'A' and fe.funid in (select funid from entidade.funcao f where f.fundsc ilike '%univer%'))
		";
	
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();

	foreach ($arrDados as $v) {
		$sql = "select ent.enbcnpj from emenda.emendadetalheentidade ed
					inner join emenda.entidadebeneficiada ent on ent.enbid = ed.enbid
				where
					ent.enbano = '".date('Y')."'
				    and ed.edeid = {$v['edeid']}";
		$cnpj = $db->pegaUm($sql);
		
		if( $cnpj ){
			$db->executar("DELETE FROM emenda.emendadetalheentidade WHERE enbid in (select enbid from emenda.entidadebeneficiada where enbcnpj = '$cnpj' and enbano = '".date('Y')."')");
			$db->executar("DELETE FROM emenda.usuarioresponsabilidade WHERE enbid in (select enbid from emenda.entidadebeneficiada where enbcnpj = '$cnpj' and enbano = '".date('Y')."')");
			$db->executar("DELETE FROM emenda.entbeneficiadacontrapartida WHERE enbid in (select enbid from emenda.entidadebeneficiada where enbcnpj = '$cnpj' and enbano = '".date('Y')."')");
			$db->executar("DELETE FROM emenda.entidadebeneficiada WHERE enbcnpj = '$cnpj' and enbano = '".date('Y')."'");
		} else {
			$db->executar("DELETE FROM emenda.emendadetalheentidade WHERE edeid = {$v['edeid']}");
		}
		$db->commit();
	}	
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
/*
function carregarUnidadeAcademico( $unicod ){
	global $db;
	
	$sql = "SELECT distinct
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
			    e.entid
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
	        	e.entunicod = '$unicod'
	            and e.entstatus = 'A'
	            and fe.funid = 12
	            and ur.pflcod = 526
	            and uss.sisid = '56' 
			    and pfu.pflcod = 526
	            and uss.suscod = 'A'
	            and e.entnumcpfcnpj is not null
	        order by
				e.entid desc";
	$arrDados = $db->pegaLinha($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	return $arrDados;
}

function carregarUnidadeElabrev( $unicod ){
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
			    case when cargo = '' then 'Outros' else cargo end as cargo,
			    usuemail
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
			        end as cargo
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
			        and ung.unicod = '$unicod'
			        and fe.funid in (select funid from entidade.funcao f where f.fundsc ilike '%univer%')
			) as foo
			where
				entnumcpfcnpj is not null";
	$arrDados = $db->pegaLinha($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	return $arrDados;
}
*/

?>